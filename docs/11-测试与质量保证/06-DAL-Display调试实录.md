# DAL Display 调试实录 — 树莓派 7" DSI 屏适配 ESP32-P4

## 硬件环境

| 项目 | 参数 |
|------|------|
| MCU | ESP32-P4 (rev 1.3) |
| 显示屏 | 树莓派 7" MIPI DSI V1 (800×480) |
| 桥接芯片 | TC358762XBG (DSI → RGB 24-bit) |
| 辅助 MCU | ATTINY88-MUR (I2C 0x45, 电源/背光/复位) |
| 触控 | FT5406 (I2C 0x38) |
| PSRAM | 32MB Hex PSRAM @ 200MHz |
| ESP-IDF | v5.5.3 |

## 问题全景图

```
总共解决 12 个问题，按出现顺序排列：
1. I2C 异步模式挂死     → trans_queue_depth=0
2. probe 超时挂死       → timeout 从 -1 改为 20ms
3. I2C 频率无效         → scl_speed_hz 从 0 改为 400000
4. DSI 时钟源 abort     → PLL_F20M 替代 XTAL
5. DSI PHY LDO 未供电   → 添加 esp_ldo_acquire_channel(3, 2500mV)
6. DPI 时钟频率为 0     → 显式传入 25.98 MHz
7. 帧缓冲 API 调用错误  → fb_num 是数量不是索引
8. DSI 主机配置缺失     → Non-Burst + HS Clock + 关闭 ACK
9. 帧缓冲分配无 DMA  → MALLOC_CAP_DMA
10. PSRAM 缓存一致性问题 → SPIRAM_USE_CAPS_ALLOC
11. 双缓冲切换干扰       → NUM_FBS=1
12. color_data 复制路径  → 直接用帧缓冲指针触发零拷贝
```

---

## 问题 1：I2C 异步模式挂死

**现象**：`I2C OK` 后设备扫描永久阻塞，无任何输出。

**日志**：
```
W (1512) i2c.master: Please note i2c asynchronous is only used for specific scenario...
I (1542) MAIN: I2C OK
（挂死）
```

**根因**：`trans_queue_depth = 4` 触发 ESP-IDF I2C 驱动异步模式。异步模式下 `i2c_master_probe()` 不兼容，调用后永久阻塞。

**解决**：`trans_queue_depth = 0`（同步模式）

```c
// main.c
.trans_queue_depth = 0,   /* 同步模式 */
```

---

## 问题 2：probe 超时挂死

**现象**：设备扫描到第一个不存在的地址 (0x01) 时挂死。

**根因**：`pal_i2c_dev_probe()` 内部调用 `i2c_master_probe(bus, addr, -1)`，超时参数 `-1` 表示无限等待。

**解决**：超时改为 20ms

```c
// pal_i2c.c
// 修改前：return i2c_master_probe(..., -1);
// 修改后：
return i2c_master_probe((i2c_master_bus_handle_t)bus, addr, 20);
```

---

## 问题 3：I2C 设备挂载失败

**现象**：`i2c_master_bus_add_device(1177): invalid scl frequency`

**根因**：`pal_i2c_dev_config_t.scl_speed_hz = 0` 在 ESP-IDF 5.5 中不合法，必须显式指定有效频率。

**解决**：`scl_speed_hz = 400000`

```c
// dal_display.c 中两处 pal_i2c_dev_attach 调用
.scl_speed_hz = 400000,
```

---

## 问题 4：DSI 时钟源 abort

**现象**：
```
abort() at _mipi_dsi_ll_set_phy_pllref_clock_source
```

**根因**：ESP32-P4 rev < 3.0 走 `#else` 分支的 LL 函数，只接受 `PLL_F20M`、`RC_FAST`、`PLL_F25M`。我们传入的 `MIPI_DSI_PHY_PLLREF_CLK_SRC_DEFAULT = SOC_MOD_CLK_XTAL` 不在其中。

**LL 函数 switch-case（`mipi_dsi_ll.h:258`）**：
```c
case MIPI_DSI_PHY_PLLREF_CLK_SRC_PLL_F20M:  // 0
case MIPI_DSI_PHY_PLLREF_CLK_SRC_RC_FAST:   // 1
case MIPI_DSI_PHY_PLLREF_CLK_SRC_PLL_F25M:  // 2
    break;
default:
    abort();  // ← SOC_MOD_CLK_XTAL(=9) 走到这里
```

**解决**：使用 `MIPI_DSI_PHY_PLLREF_CLK_SRC_DEFAULT_LEGACY` (= PLL_F20M)

```c
// pal_mipi_dsi.c
.phy_clk_src = MIPI_DSI_PHY_PLLREF_CLK_SRC_DEFAULT_LEGACY,
```

---

## 问题 5：DSI PHY LDO 未供电

**现象**：`esp_lcd_new_dsi_bus()` 调用后挂死（Watchdog 超时）。

**根因**：ESP32-P4 的 MIPI DSI PHY 需要 LDO 供电（通道 3, 2500mV）才能从 "No Power" 进入 "Shutdown" 状态。必须**在创建 DSI 总线之前**完成。

**参考**：ESP-IDF 示例 `examples/peripherals/lcd/mipi_dsi/`

**解决**：在 `esp_lcd_new_dsi_bus()` 前调用 `esp_ldo_acquire_channel()`

```c
// pal_mipi_dsi.c
esp_ldo_channel_config_t ldo_cfg = {
    .chan_id    = 3,
    .voltage_mv = 2500,
};
esp_ldo_acquire_channel(&ldo_cfg, NULL);
```

---

## 问题 6：DPI 时钟频率为 0

**现象**：`esp_lcd_new_panel_dpi(172): invalid DPI clock frequency 0.00`

**根因**：设置 `dpi_clock_freq_mhz = 0` 期望驱动自动计算，但 ESP-IDF 5.5.3 不接受 0。

**解决**：显式传入 25.98 MHz（匹配 Linux 内核驱动）

```c
// main.c
#define DPI_CLK_MHZ  (25.98f)

// pal_mipi_dsi_config_t 新增字段
float dpi_clock_freq_mhz;
```

---

## 问题 7：帧缓冲 API 调用错误

**现象**：`esp_lcd_dpi_panel_get_frame_buffer(409): invalid frame buffer number`

**根因**：`esp_lcd_dpi_panel_get_frame_buffer` 的第二个参数 `fb_num` 是**要获取的缓冲数量**（≥ 1），不是索引。我们错误地循环调用 `fb_num=0,1,2...`。

**API 签名**：
```c
esp_err_t esp_lcd_dpi_panel_get_frame_buffer(
    esp_lcd_panel_handle_t panel,
    uint32_t fb_num,     // 数量！不是索引！
    void **fb0, ...      // fb_num 个输出指针
);
```

**校验逻辑**：
```c
ESP_RETURN_ON_FALSE(fb_num && fb_num <= dpi_panel->num_fbs, ...);
// fb_num=0 直接失败！
```

**解决**：一次性传入正确数量
```c
// 修改前（错误）：
for (int i = 0; i < ctx->fb_count; i++) {
    esp_lcd_dpi_panel_get_frame_buffer(ctx->panel, i, &ctx->fb[i], NULL);
}

// 修改后（正确）：
if (ctx->fb_count == 2) {
    esp_lcd_dpi_panel_get_frame_buffer(ctx->panel, 2, &ctx->fb[0], &ctx->fb[1]);
} else if (ctx->fb_count == 1) {
    esp_lcd_dpi_panel_get_frame_buffer(ctx->panel, 1, &ctx->fb[0]);
}
```

---

## 问题 8：DSI 主机配置缺失（白屏根因 #1）

**现象**：屏幕全白，`E lcd.dsi: can't fetch data from external memory fast enough, underrun happens`

**根因**：缺少 4 个关键的 DSI 主机控制器配置，TC358762 桥接器无法正确接收和解析视频数据：

| 缺失配置 | 影响 |
|---------|------|
| Non-Burst + Sync Pulses 视频模式 | TC358762 仅支持此模式，其他模式数据错乱 |
| Frame ACK 关闭 | TC358762 不响应 ACK，主机等待超时 |
| DSI 时钟 HS 连续模式 | TC358762 FLL 需要稳定参考时钟 |
| CMD ACK (BTA) 关闭 | 避免 LP（低功耗）通道竞争 |

**解决**：在 `esp_lcd_panel_init()` 前后添加 DSI 主机配置

```c
// pal_mipi_dsi.c — 面板 init 前
{
    struct esp_lcd_dsi_bus_t *bus = (struct esp_lcd_dsi_bus_t *)ctx->dsi_bus;
    // Non-Burst + Sync Pulses
    mipi_dsi_host_ll_dpi_set_video_burst_type(bus->hal.host,
        MIPI_DSI_LL_VIDEO_NON_BURST_WITH_SYNC_PULSES);
    // 关闭 Frame ACK
    mipi_dsi_host_ll_dpi_enable_frame_ack(bus->hal.host, false);
}
esp_lcd_panel_init(ctx->panel);
// 面板 init 后
{
    struct esp_lcd_dsi_bus_t *bus = (struct esp_lcd_dsi_bus_t *)ctx->dsi_bus;
    // 强制 HS 连续时钟
    mipi_dsi_host_ll_set_clock_lane_state(bus->hal.host,
        MIPI_DSI_LL_CLOCK_LANE_STATE_HS);
    // 关闭 CMD ACK/BTA
    mipi_dsi_host_ll_enable_cmd_ack(bus->hal.host, false);
}
```

> 需要访问 ESP-IDF 私有头文件 `mipi_dsi_priv.h`，在 PAL CMakeLists.txt 中添加：
> ```cmake
> INCLUDE_DIRS ... "${IDF_PATH}/components/esp_lcd/dsi"
> ```

---

## 问题 9：帧缓冲分配无 DMA 属性（白屏根因 #2）

**现象**：帧缓冲数据写入正确（CPU 回读验证通过），但屏幕始终显示初始值（白色/蓝色），颜色切换无效。

**验证数据**：
```
I (1771) MAIN: 黑 msync=0 fb[0]=00          ← 写入了 0x00（黑）
I (3791) MAIN: 红 msync=0 fb[0]=00,00,ff    ← 写入了 RGB(255,0,0)（红）
I (5811) MAIN: 绿 msync=0 fb[0]=00,ff,00    ← 写入了 RGB(0,255,0)（绿）
```
CPU 回读数据正确，但屏幕不变色 → **缓存一致性问题**。

**根因**：ESP32-P4 的 PSRAM 存在 L1/L2 两级缓存：
```
CPU 写 → L1 Cache → L2 Cache → PSRAM
GDMA 读 ──────────────────────→ PSRAM (绕过缓存！)
```

`esp_cache_msync(DIR_C2M)` 只写回 L1 → L2，但 **L2 Cache 不会自动写回 PSRAM**。GDMA 直接读 PSRAM 看到的仍是旧数据。

`MALLOC_CAP_SPIRAM | MALLOC_CAP_DEFAULT` 分配的 PSRAM 内存不走 DMA 兼容路径（带缓存），导致 GDMA 读到脏数据。

**解决**：
1. sdkconfig 切换 `SPIRAM_USE_MALLOC → SPIRAM_USE_CAPS_ALLOC`
2. 帧缓冲分配使用 `MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA`

```c
// esp_lcd_panel_dpi.c:231 修改后
uint8_t *frame_buffer = heap_caps_calloc(1, fb_size,
    MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
```

```ini
# sdkconfig
CONFIG_SPIRAM_USE_CAPS_ALLOC=y
# CONFIG_SPIRAM_USE_MALLOC is not set
# CONFIG_SPIRAM_ALLOW_STACK_EXTERNAL_MEMORY is not set
```

---

## 问题 10：TC358762 芯片 ID 不匹配

**现象**：`W (1532) TC358762: 芯片 ID 不匹配: 期望 0x0044, 实际 0x00008B8B`

**根因**：TC358762 的 I2C 寄存器读取**始终返回 0x8B8B**，这是芯片的已知限制。寄存器配置应通过 DSI Generic Long Write 数据包完成，而非 I2C。

**解决**：tc358762 驱动完全重写，寄存器写改用 `pal_mipi_dsi_send_generic_write()`（DSI Generic Long Write, DT=0x29）

```c
// tc358762.c — 通过 DSI 写寄存器
static int tc358762_write_reg(tc358762_handle_t handle,
                              uint16_t addr, uint32_t val)
{
    uint8_t payload[6] = {
        (addr >> 0) & 0xFF, (addr >> 8) & 0xFF,
        (val  >> 0)  & 0xFF, (val  >> 8)  & 0xFF,
        (val  >> 16) & 0xFF, (val  >> 24) & 0xFF,
    };
    return pal_mipi_dsi_send_generic_write(d->dsi, d->vc, payload, 6);
}
```

---

## 问题 11：ATTINY88 寄存器地址错误

**现象**：`ATTINY88 I2C 挂载失败: 258`，且无法正确控制电源/背光。

**根因**：ATTINY88 固件 v2 (ID=0xC3) 的寄存器地址为 **0x80-0x86**，而初版代码用了 0x00-0x05。

**正确寄存器映射**：

| 寄存器 | 地址 | 功能 |
|--------|------|------|
| REG_ID | 0x80 | 固件 ID (0xC3=v2) |
| REG_PORTA | 0x81 | 扫描方向 (bit2=左→右) |
| REG_PORTB | 0x82 | 主电源 (bit7=开) |
| REG_PORTC | 0x83 | 复位控制 (LED/触控/LCD/桥) |
| REG_PWM | 0x86 | 背光 PWM (0~255) |

---

## 问题 12：I2C 地址分配确认

**扫描结果**：
```
I (1511) MAIN:   dev: 0x10    ← IMX219 摄像头
I (1521) MAIN:   dev: 0x18    ← TC358762 (仅 probe 可用，寄存器不可读)
I (1521) MAIN:   dev: 0x38    ← FT5406 触控
I (1521) MAIN:   dev: 0x45    ← ATTINY88 背光/电源 MCU
```

**初版代码错误**：将 0x45 当作 TC358762，0x18 当作 ATtiny88（**完全反了**）。

---

## 最终初始化流程

```
1. I2C 总线初始化 (SDA=7, SCL=8, 400kHz, 同步模式)
2. ATtiny88 (0x45) 初始化 → 读 ID → 主电源上电（桥保持复位）
3. DSI PHY LDO 供电 (通道3, 2500mV)
4. 创建 MIPI DSI 总线 (1-lane, 600Mbps, PLL_F20M)
5. DSI 主机预配置: Non-Burst + 关闭 Frame ACK
6. 创建 DPI 面板 (800×480, RGB888, 25.98MHz, 1 帧缓冲)
7. DSI 主机后配置: HS 连续时钟 + 关闭 CMD ACK
8. ATtiny88 释放桥复位 + LCD 复位
9. TC358762 寄存器初始化（通过 DSI Generic Long Write）
10. 背光点亮
```

## ESP-IDF 源码修改清单

| 文件 | 行 | 修改 | 原因 |
|------|-----|------|------|
| `esp_lcd/dsi/esp_lcd_panel_dpi.c` | 231 | `MALLOC_CAP_DEFAULT` → `MALLOC_CAP_DMA` | PSRAM 帧缓冲 DMA 对齐 |
| `sdkconfig` | — | `SPIRAM_USE_MALLOC` → `SPIRAM_USE_CAPS_ALLOC` | PSRAM 支持 DMA 分配 |
| `esp_lcd/dsi/esp_lcd_panel_dpi.c:231` | — | `heap_caps_calloc(..., MALLOC_CAP_SPIRAM \| MALLOC_CAP_DMA)` | — |

## 关键经验

1. **ESP32-P4 PSRAM + GDMA = 必须用 `MALLOC_CAP_DMA`**，否则 L2 缓存不一致导致 DMA 读到脏数据
2. **ESP-IDF API 细节陷阱**：`get_frame_buffer` 的 `fb_num` 是数量不是索引；`scl_speed_hz` 不能为 0
3. **DSI 主机配置和 DSI 总线配置同样重要**：缺 Non-Burst + HS Clock + 关闭 ACK 这些"非面板"配置导致白屏
4. **LDO 供电先于外设初始化**：ESP32-P4 DSI PHY 需要 LDO 通道 3 供电
5. **参考已有实现**：避免凭空猜测寄存器值和初始化顺序