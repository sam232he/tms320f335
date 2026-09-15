# drive.pdf Agent-Readable Hardware Reference

Source: `drive.pdf`  
Generated from the Altium schematic PDF dated 2026-09-09.  
Use this file to check firmware pin configuration against the schematic.

## High-Risk Firmware Checklist

### ADC Full-Scale Assumption

TMS320F28335 ADC conversion range is 0 V to 3 V:

```text
adc_code = 4096 * (adc_pin_voltage - ADCLO) / 3.0
adc_pin_voltage = adc_code * 3.0 / 4096
```

Use 0..4095 clipping in firmware. Values at or above 3.0 V saturate.

### DSP PWM To Motor Driver

The note on the DSP sheet says:

- Motor 1: EPWM6
- Motor 2: EPWM5

Actual schematic net mapping:

| Firmware net | TMS320F28335 pin | DSP GPIO/function on pin | Isolator input | Isolated net | DRV8432 input |
|---|---:|---|---|---|---|
| `DSP_EPWM1_A` | 19 | `GPIO10/EPWM6A/CANRXB/ADCSOCBO` | U59 `VI` | `ISO_PWM_A` | `PWM_A` |
| `DSP_EPWM1_B` | 20 | `GPIO11/EPWM6B/SCIRXDB/ECAP4` | U61 `VI` | `ISO_PWM_B` | `PWM_B` |
| `DSP_EPWM2_A` | 18 | `GPIO9/EPWM5B/SCITXDB/ECAP3` | U63 `VI` | `ISO_PWM_C` | `PWM_C` |
| `DSP_EPWM2_B` | 17 | `GPIO8/EPWM5A/CANTXB/ADCSOCAO` | U64 `VI` | `ISO_PWM_D` | `PWM_D` |

Important: The schematic net names `DSP_EPWM2_A` and `DSP_EPWM2_B` are not on an ePWM2 peripheral. They are on ePWM5 pins.

### ADC Inputs

| Measured signal | Sheet net | DSP ADC net | TMS320F28335 pin | ADC channel | Front-end shown in schematic |
|---|---|---|---:|---|---|
| Current sensor 1 output | `IOUT_1` | `DSP_ADCINA0` | 42 | `ADCINA0` | Direct ACS711 output, no divider shown |
| Pressure sensor output | `PS_OUT` | `DSP_ADCINA1` | 41 | `ADCINA1` | MPXHZ6400AC6T1 output through 0 ohm link, no divider shown |
| Current sensor 2 output | `IOUT_2` | `DSP_ADCINB0` | 46 | `ADCINB0` | Direct ACS711 output, no divider shown |

ADC resistor-divider check:

- `PS_OUT`: U7 `VOUT` has C144 47 pF to GND and R51 51 k to GND. R50 is 0 ohm in series/path to exported `PS_OUT`. R52 is DNP to GND. This is not a resistor divider as populated in the schematic.
- `IOUT_1` and `IOUT_2`: ACS711 outputs go to DSP ADC nets through harness labels. No divider is shown in `drive.pdf`.
- Since the pressure sensor is powered from +5 V but connected to a DSP ADC input with no populated divider, this needs special hardware/firmware sanity checking.

### ADC Scale Equations

#### ACS711 Current Sensors

Schematic part: `ACS711ELCTR-12AB-T`.

Datasheet scale at VCC = 3.3 V:

- Range: +/-12.5 A
- Sensitivity: 110 mV/A
- Zero-current output: VCC x 0.5 = 1.65 V nominal
- Positive current direction: from IP+ pins 1/2 to IP- pins 3/4

Firmware conversion, assuming ADCLO = 0 V:

```text
adc_v = adc_code * 3.0 / 4096
current_a = (adc_v - 1.65) / 0.110
```

Equivalent direct formula:

```text
current_a = ((adc_code * 3.0 / 4096) - 1.65) / 0.110
```

Important saturation warning:

- At +12.5 A, nominal ACS711 output is 1.65 + 12.5 * 0.110 = 3.025 V.
- The DSP ADC full scale is 3.0 V, so positive current clips at about `(3.0 - 1.65) / 0.110 = 12.27 A`.
- At -12.5 A, nominal output is 0.275 V, which is inside the ADC range.

#### MPXHZ6400 Pressure Sensor

Schematic part: `MPXHZ6400AC6T1`, powered from +5 V.

Datasheet transfer function:

```text
sensor_vout = VS * (0.002421 * pressure_kpa - 0.00842)
```

For VS = 5.0 V:

```text
sensor_vout = 0.012105 * pressure_kpa - 0.0421
```

Firmware conversion if connected directly to ADC, assuming ADCLO = 0 V and no saturation:

```text
adc_v = adc_code * 3.0 / 4096
pressure_kpa = ((adc_v / 5.0) + 0.00842) / 0.002421
```

Critical hardware warning:

- Sensor output range is about 0.2 V to 4.8 V over 20 kPa to 400 kPa.
- The DSP ADC input range is 0 V to 3 V.
- With no populated divider, the ADC reaches 3.0 V at about 251 kPa:

```text
pressure_at_adc_full_scale = ((3.0 / 5.0) + 0.00842) / 0.002421 = 251.3 kPa
```

- Above about 251 kPa the ADC reading saturates.
- Above 3.0 V at the ADC pin is outside the stated ADC input range; this is a hardware risk, not only a firmware scaling issue.

If a divider is later populated, use:

```text
adc_v = sensor_vout * divider_ratio
divider_ratio = Rbottom / (Rtop + Rbottom)
sensor_vout = adc_v / divider_ratio
pressure_kpa = (((adc_v / divider_ratio) / VS) + 0.00842) / 0.002421
```

No such populated divider is shown in `drive.pdf`.

### Scale Sources Used

| Item | Source |
|---|---|
| TMS320F28335 ADC range/formula | Local `tms320f28335_datasheet.pdf`, ADC chapter page 447 |
| ACS711 current scale | Allegro ACS711 datasheet, part `ACS711ELCTR-12AB-T` |
| MPXHZ6400 pressure transfer | NXP/Freescale MPXHZ6400A datasheet |


### Fault, OTW, Reset

| Firmware net | TMS320F28335 pin | DSP GPIO/function on pin | Direction at DSP | Isolated/motor-driver side |
|---|---:|---|---|---|
| `OTW` | 21 | `GPIO12/TZ1/CANTXB/MDXB` | Input from motor driver | `ISO_OTW` from DRV8432 `OTW` |
| `FAULT` | 24 | `GPIO13/TZ2/CANRXB/MDRB` | Input from motor driver | `ISO_FAULT` from DRV8432 `FAULT` |
| `RST_AB` | 72 | `GPIO26/ECAP3/EQEP2I/MCLKXB` | Output to motor driver | `ISO_RST_AB` to DRV8432 `RESET_AB` |
| `RST_CD` | not connected on DSP sheet | not connected | Not available | Shown as not connected in harness |

## DSP Pin Map

Sheet: `DSP.SchDoc`, page 3.

### PWM Pins

| Net | TMS320F28335 pin | GPIO/function |
|---|---:|---|
| `DSP_EPWM1_A` | 19 | `GPIO10/EPWM6A/CANRXB/ADCSOCBO` |
| `DSP_EPWM1_B` | 20 | `GPIO11/EPWM6B/SCIRXDB/ECAP4` |
| `DSP_EPWM2_A` | 18 | `GPIO9/EPWM5B/SCITXDB/ECAP3` |
| `DSP_EPWM2_B` | 17 | `GPIO8/EPWM5A/CANTXB/ADCSOCAO` |

Unused nearby ePWM-capable pins on schematic:

| TMS320F28335 pin | GPIO/function | Schematic status |
|---:|---|---|
| 5 | `GPIO0/EPWM1A` | No connect |
| 6 | `GPIO1/EPWM1B/ECAP6/MFSRB` | No connect |
| 7 | `GPIO2/EPWM2A` | No connect |
| 10 | `GPIO3/EPWM2B/ECAP5/MCLKRB` | No connect |
| 11 | `GPIO4/EPWM3A` | No connect |
| 12 | `GPIO5/EPWM3B/MFSRA/ECAP1` | No connect |
| 13 | `GPIO6/EPWM4A/EPWMSYNCI/EPWMSYNCO` | No connect |
| 16 | `GPIO7/EPWM4B/MCLKRA/ECAP2` | No connect |

### ADC Pins

| Net | TMS320F28335 pin | ADC channel |
|---|---:|---|
| `DSP_ADCINA0` | 42 | `ADCINA0` |
| `DSP_ADCINA1` | 41 | `ADCINA1` |
| `DSP_ADCINB0` | 46 | `ADCINB0` |

Other ADC pins `ADCINA2..ADCINA7` and `ADCINB1..ADCINB7` are shown no-connect in this schematic.

### UART / SCI Pins

| Net | TMS320F28335 pin | GPIO/function | Harness mapping |
|---|---:|---|---|
| `DSP_TX1` | 148 | `GPIO35/SCITXDA/XR/W` | `DSP_UART_1` -> `RS422_TX1` |
| `DSP_RX1` | 145 | `GPIO36/SCIRXDA/XZCS0` | `DSP_UART_1` -> `RS422_RX1` |
| `DSP_TX2` | 25 | `GPIO14/TZ3/XHOLD/SCITXDB/MCLKXB` | `DSP_UART_2` -> `RS422_TX2` |
| `DSP_RX2` | 26 | `GPIO15/TZ4/XHOLDA/SCIRXDB/MFSXB` | `DSP_UART_2` -> `RS422_RX2` |
| `DSP_TX3` | 114 | `GPIO63/SCITXDC/XD16` | `DSP_UART_3` -> `RS422_TX3` |
| `DSP_RX3` | 113 | `GPIO62/SCIRXDC/XD17` | `DSP_UART_3` -> `RS422_RX3` |

### CAN Pins

| Net | TMS320F28335 pin | GPIO/function | Harness mapping |
|---|---:|---|---|
| `DSP_CAN_TX` | 27 | `GPIO16/SPISIMOA/CANTXB/TZ5` | `DSP_CAN_TX` -> `CAN1_TX` after isolation |
| `DSP_CAN_RX` | 28 | `GPIO17/SPISOMIA/CANRXB/TZ6` | `DSP_CAN_RX` -> `CAN1_RX` after isolation |

### Encoder Pins

| Net | TMS320F28335 pin | GPIO/function | Encoder sheet |
|---|---:|---|---|
| `ENC1_data` | 130 | `GPIO73/XD6` | MAX490 receiver output `ENC1_data` |
| `ENC1_clk` | 129 | `GPIO72/XD7` | MAX490 receiver output `ENC1_clk` |
| `ENC2_data` | 122 | `GPIO67/XD12` | MAX490 receiver output `ENC2_data` |
| `ENC2_clk` | 119 | `GPIO66/XD13` | MAX490 receiver output `ENC2_clk` |

### LEDs

| Net | TMS320F28335 pin | GPIO/function |
|---|---:|---|
| `LED1` | 131 | `GPIO74/XD5` |
| `LED2` | 132 | `GPIO75/XD4` |
| `LED3` | 133 | `GPIO76/XD3` |
| `LED4` | 134 | `GPIO77/XD2` |
| `LED5` | 135 | `GPIO78/XD1` |
| `LED6` | 136 | `GPIO79/XD0` |

### EEPROM Pins

| Net | TMS320F28335 pin | GPIO/function |
|---|---:|---|
| `EEPROM_SDA` | 74 | `GPIO32/SDAA/EPWMSYNCI/ADCSOCAO` |
| `EEPROM_SCL` | 75 | `GPIO33/SCLA/EPWMSYNCO/ADCSOCBO` |
| `EEPROM_A1` | 66 | `GPIO22/EQEP1S/MCLKXA/SCITXDB` |
| `EEPROM_WP` | 67 | `GPIO23/EQEP1I/MFSXA/SCIRXDB` |

EEPROM part on sheet: CAT24C256WI-GT3. Pull-ups on SCL/SDA are 5.1 k to +3V3.

### Boot / External Address Pins

| Net | TMS320F28335 pin | GPIO/function |
|---|---:|---|
| `DSP_XA12` | 169 | `GPIO84/XA12` |
| `DSP_XA13` | 172 | `GPIO85/XA13` |
| `DSP_XA14` | 173 | `GPIO86/XA14` |
| `DSP_XA15` | 174 | `GPIO87/XA15` |

Boot strap resistors on sheet:

| Net | Pull-up | Pull-down |
|---|---|---|
| `DSP_XA12` | R16 20 k to +3V3 | R40 DNP to GND |
| `DSP_XA13` | R37 20 k to +3V3 | R41 DNP to GND |
| `DSP_XA14` | R38 20 k to +3V3 | R42 DNP to GND |
| `DSP_XA15` | R39 20 k to +3V3 | R43 DNP to GND |

## Motor Driver Sheet

Sheet: `MOTOR_DRIVER.SchDoc`, page 2.

### DRV8432 Inputs / Outputs

Part: DRV8432DKD / DRV8432.

| DRV8432 signal | Schematic net | Notes |
|---|---|---|
| `PWM_A` | `ISO_PWM_A` | From DSP `DSP_EPWM1_A` through U59 |
| `PWM_B` | `ISO_PWM_B` | From DSP `DSP_EPWM1_B` through U61 |
| `PWM_C` | `ISO_PWM_C` | From DSP `DSP_EPWM2_A` through U63 |
| `PWM_D` | `ISO_PWM_D` | From DSP `DSP_EPWM2_B` through U64 |
| `OTW` | `ISO_OTW` | To DSP `OTW` through U60 |
| `FAULT` | `ISO_FAULT` | To DSP `FAULT` through U62 |
| `RESET_AB` | `ISO_RST_AB` | From DSP `RST_AB` through U65 |
| `RESET_CD` | no connected DSP net shown | `RST_CD` appears not connected in harness |
| `OUT_A` | `OUTPUT_A` / motor connector `OUT_A` | Current sensor U2 path |
| `OUT_B` | `OUT_B` | Motor connector |
| `OUT_C` | `OUT_C` | Motor connector |
| `OUT_D` | `OUTPUT_D` / motor connector `OUT_D` | Current sensor U3 path |

### Motor Connector

Connector: header 5x2.

| Connector pins | Net |
|---|---|
| 1, 3 | `OUT_A` |
| 5, 7 | `OUT_B` |
| 9, 11 | `OUT_C` |
| 13, 15 | `OUT_D` |

### Current Sensors

Parts: ACS711ELCTR-12AB-T.

| Sensor | Current path pins | Sense output | Fault output | Supply/decoupling |
|---|---|---|---|---|
| U2 | `OUTPUT_A` on IP+ pins 1/2, `OUT_A` on IP- pins 3/4 | `IOUT_1` on pin 7 | `FAULT_1` on pin 6, R16 1 k pull-up to +3V3 | +3V3 on pin 8, C21 100 nF and C22 10 uF to GND |
| U3 | `OUT_D` on IP+ pins 1/2, `OUTPUT_D` on IP- pins 3/4 | `IOUT_2` on pin 7 | `FAULT_2` on pin 6, R17 1 k pull-up to +3V3 | +3V3 on pin 8, C23 100 nF and C24 10 uF to GND |

Harness maps `IOUT_1 -> DSP_ADCINA0`, `IOUT_2 -> DSP_ADCINB0`, `FAULT_1 -> FAULT_1`, `FAULT_2 -> FAULT_2`.

## Pressure Sensor Sheet

Sheet: `PRESSURE_SENSOR.SchDoc`, page 4.

Part: MPXHZ6400AC6T1.

| Signal | Part pin | Net / component |
|---|---:|---|
| `VS` | 2 | +5 V |
| `GND` | 3 | GND |
| `VOUT` | 4 | Sensor output node |
| Output filter/load | - | C144 47 pF to GND, R51 51 k to GND |
| Series/export link | - | R50 0 ohm from output node to `PS_OUT` |
| Optional shunt | - | R52 DNP from `PS_OUT` to GND |
| DSP connection | - | `PS_OUT -> DSP_ADCINA1` |

Important ADC note: populated schematic does not show a divider from the 5 V pressure sensor output to the DSP ADC input.

## Encoder Sheet

Sheet: `ENCODER.SchDoc`, page 5.

### Encoder 1

| Connector / signal | Receiver input | Receiver output | DSP net |
|---|---|---|---|
| `ENC_U1_data_P/N` | MAX490 U17 A/B side through 120 ohm series resistors | `ENC1_data` | Pin 130 / `GPIO73/XD6` |
| `ENC_U1_clk_P/N` | MAX490 U17 Y/Z side through 120 ohm series resistors | `ENC1_clk` | Pin 129 / `GPIO72/XD7` |

### Encoder 2

| Connector / signal | Receiver input | Receiver output | DSP net |
|---|---|---|---|
| `ENC_U2_data_P/N` | MAX490 U16 A/B side through 120 ohm series resistors | `ENC2_data` | Pin 122 / `GPIO67/XD12` |
| `ENC_U2_clk_P/N` | MAX490 U16 Y/Z side through 120 ohm series resistors | `ENC2_clk` | Pin 119 / `GPIO66/XD13` |

## Serial And CAN Sheet

Sheet: `SERIAL.SchDoc`, page 6.

### RS422

Each UART channel uses an isolated RS422 transceiver section. Harness names:

| DSP-side net | Isolated/connector-side net |
|---|---|
| `DSP_TX1` | `RS422_TX1` |
| `DSP_RX1` | `RS422_RX1` |
| `DSP_TX2` | `RS422_TX2` |
| `DSP_RX2` | `RS422_RX2` |
| `DSP_TX3` | `RS422_TX3` |
| `DSP_RX3` | `RS422_RX3` |

### CAN

| DSP-side net | Isolated/connector-side net |
|---|---|
| `DSP_CAN_TX` | `CAN1_TX` |
| `DSP_CAN_RX` | `CAN1_RX` |

CAN isolation/transceiver block uses isolated +5V_CAN/GND_CAN domain.

## Isolator Sheet

Sheet: `ISOLATOR.SchDoc`, page 7.

Optocouplers: ACPL-077L-500E.

| Direction | Input net | Output net | Device |
|---|---|---|---|
| DSP -> motor driver | `DSP_EPWM1_A` | `ISO_PWM_A` | U59 |
| DSP -> motor driver | `DSP_EPWM1_B` | `ISO_PWM_B` | U61 |
| DSP -> motor driver | `DSP_EPWM2_A` | `ISO_PWM_C` | U63 |
| DSP -> motor driver | `DSP_EPWM2_B` | `ISO_PWM_D` | U64 |
| DSP -> motor driver | `RST_AB` | `ISO_RST_AB` | U65 |
| Motor driver -> DSP | `OTW` / `ISO_OTW` | `OTW` at DSP | U60 |
| Motor driver -> DSP | `FAULT` / `ISO_FAULT` | `FAULT` at DSP | U62 |

Harness test points:

| Net | Test point |
|---|---|
| `DSP_EPWM1_A` | TP24 |
| `DSP_EPWM2_A` | TP25 |
| `DSP_EPWM1_B` | TP26 |
| `DSP_EPWM2_B` | TP27 |
| `OTW` | TP28 |
| `FAULT` | TP29 |
| `RST_AB` | TP30 |

## Power Sheet

Sheet: `PWR.SchDoc`, page 8.

| Rail | Main source / part | Notes |
|---|---|---|
| +5 V | MIW102-24505 from +24 V | Output uses multiple capacitors and BNX filter before +5 V rail |
| +12 V | M78AR12-1 from +24 V | Output rail +12 V |
| +3V3_NI | M78AR033-1 from +24 V | Non-isolated/NI 3.3 V rail |
| +1V9 | TPS767D301MPWPREP channel | Generated from +5 V |
| +3V3 | TPS767D301MPWPREP channel | Generated from +5 V |

Power note shown on schematic: "1RESET, 2RESET RESISTOR AND LOGIC CHECK."

## Page Index

| PDF page | Sheet / content |
|---:|---|
| 1 | Top-level sheet |
| 2 | `MOTOR_DRIVER.SchDoc` |
| 3 | `DSP.SchDoc` |
| 4 | `PRESSURE_SENSOR.SchDoc` |
| 5 | `ENCODER.SchDoc` |
| 6 | `SERIAL.SchDoc` |
| 7 | `ISOLATOR.SchDoc` |
| 8 | `PWR.SchDoc` |
