---
document_type: product_catalog_datasheet
manufacturer: "吉林省三晟传感技术有限公司 (Jilin Sansheng Sensing Technology Co., Ltd.)"
brand: SANSHENG
category: rotary_position_encoders
product_series: [TSZ, SZM, TSM, SE]
source_file: encoder.pdf
source_pages_total: 15 (rendered as 2-page spreads; PDF has no embedded text layer — vector/CorelDRAW artwork, OCR'd via visual inspection)
extraction_method: page rasterization + visual reading (no text layer present in source PDF)
language_of_source: Chinese (zh-CN), with partial English labels
units: metric (mm, kg, °C) unless noted
currency: not stated in source
last_verified: 2026-09-09
---

# ⚠️ Source-Fidelity Notes (read before using this document)

- The source PDF (`encoder.pdf`, 15 pages) contains **no extractable text layer** — every page is vector artwork exported from CorelDRAW. All data below was obtained by rasterizing each page at 150–300 DPI and reading it visually, then cross-checking ambiguous cells at 300 DPI.
- The source file **duplicates four page-spreads** (pages numbered 09/10, 13/14, 17/18, and 21/22 each appear twice in a row, pixel-identical apart from JPEG re-encoding noise). No unique content was found on the duplicate pages.
- The table of contents (see below) lists a final section **"24 联系方式" (Contact Information)** that is **not present anywhere in the 15-page file** — it appears to have been replaced by one of the duplicate spreads during export. **No contact details (phone, address, email, website) exist in this source document.** Do not fabricate any.
- A few dimension-table cells print a diameter symbol (Φ) in a column that is otherwise a plain length value (e.g., TSZ70 and TSZ130 rows in the "h1" column). These are transcribed exactly as printed; they are likely a formatting inconsistency in the original artwork rather than a real design difference — flag to a human if precision matters.
- One printed spec cell ("工作温度 / Operating Temperature") shows two stacked low-end values ("−40" and "−55") over a single high-end value ("+65"). This most likely encodes **two temperature grades sharing the same upper bound**: standard −40°C~+65°C and extended −55°C~+65°C. Transcribed with this caveat wherever it occurs.
- The SE series' solid-shaft ("实心轴") dimension table lists only 2 models (SE38S6, SE58S10), while the SE series master spec table lists 8 models, most of which are hollow-shaft ("空心轴") types documented separately. This is reproduced as printed — it is a gap in the source document, not an extraction error.
- All part-ordering ("选型表" / ordering code) tables are transcribed segment-by-segment as printed. Where a segment appears to bundle two conceptual fields (e.g., a mounting-type letter prefixed onto a shaft-diameter number, such as `T50`), this is called out explicitly rather than silently split.

---

# 1. Company Profile (企业简介)

**Sansheng Sensing Technology Co., Ltd. (Jilin Province)** is a high-tech enterprise integrating R&D, manufacturing, and sales of rotary encoders.

| Field | Value |
|---|---|
| Company | 吉林省三晟传感技术有限公司 |
| Years in encoder industry | 20+ |
| Annual encoder production | ~50,000 units/year |
| Research partnerships | 50+ research institutes |
| Industries served | China Ordnance (中国兵器), China Shipbuilding Heavy Industry (中船重工), China Aerospace Science & Industry (中国航天科工), China Electronics Technology Group (中国电科), and others |
| Core claim | First-to-market ("行业首创") ultra-thin integrated bearing-embedded absolute encoder ("一体式含轴承绝对值编码器") |
| Positioning accuracy claim (marketing) | up to ±2 arcsec |
| Quality system | GJB 9001C-2017 |
| Inspection | 100% environmental and vibration testing on all units |
| Partner/customer logos shown | CNNC (中核集团), CASC (中国航天), AVIC (中国航空), CSGC (中国兵装/CSGC), CSIC (中船重工), CETC (中国电科), CEC (中国电子) |

## 1.1 Customization Program (量身定制)

| Attribute | Value |
|---|---|
| Quote turnaround | Within 8 hours |
| Sample delivery | Within 25 days |
| Customizable dimensions | Yes — size (尺寸定制) |
| Customizable protocol | Yes (协议定制) |
| Domestic-substitution customization | Yes (国产化定制) |
| Special-environment customization | Yes (特殊环境定制) |
| Integrated drive+control customization | Yes (驱控一体方案定制) |
| High-pulse-count incremental encoder customization | Yes (高脉冲增量编码器定制) |

## 1.2 Why Sansheng (选择三晟)

- 20+ years focused on encoders (深耕编码器行业20余年)
- Manufacturer, not reseller (制造商)
- Worry-free after-sales (售后无忧)
- High precision (高精度)
- In-stock supply available (现货供应)
- Custom solution design (方案定制)
- Simple installation (安装简易)
- Independently controllable / self-sufficient supply chain (自主可控)

## 1.3 Standards Compliance (标准)

Executed under **GJB 9001C-2017** quality management system.

| Standard type | Standard number |
|---|---|
| Corporate quality management | GJB 9001C-2017 |
| Product/industry standard | JB/T 9942-2012 |
| Precision/accuracy testing standard | EF-0003, JJF 1115-2004 |
| Environmental test standard | GJB 1032A-2020, GJB150.18A-2009 |

## 1.4 Patent

- Patent claim: "专利产品 仿冒必究" (Patented product — counterfeiting will be prosecuted)
- Patent number: **ZL202420254137.9** (applies to the ultra-thin integrated encoder structure, see §2.2)

---

# 2. Application Areas & Mounting Concept (applies across the whole catalog)

## 2.1 Application Domains (应用领域)

Encoders in this catalog are marketed for motion-control feedback in:

| Application (Chinese) | Application (English) |
|---|---|
| 吊舱 | Gimbal / sensor pods (e.g., aircraft belly pods) |
| 转台 | Turntables / rotary stages |
| 自行武器 | Self-propelled weapon systems |
| 雷达 | Radar |
| 导引头 | Seeker heads |
| 船舶 | Marine / shipboard systems |
| 武器站 | Weapon stations |

## 2.2 "Ultra-thin Integrated Encoder" Structure & Quick Install

Marketing name: 超薄一体化编码器 ("ultra-thin integrated bearing-embedded absolute encoder"), Patent No. ZL202420254137.9.

**Key structural claims:**
- No secondary calibration required after installation ("无需二次校准" — shown crossed out over a *competitor's* two-piece calibration step, i.e., this is a claimed advantage, not a limitation)
- No height adjustment required ("无需调整高度")
- Integrated components (labeled in exploded-view drawing): seal ring (密封圈), bearing (轴承), elastic/spring flange (弹性法兰) with a rated elastic deflection/float of **±1 mm** (弹性量±1mm)

**Quick-install procedure (generic, applies to the flange-mount integrated designs — TSZ/SZM Y-type and T-type):**
1. Align the encoder's main shaft with the shaft to be measured and gently push it into place (编码器主轴对准被测轴并轻轻推入).
2. Secure using the mounting screws (利用螺钉紧固).

More detailed, model-specific installation notes are given per product series below (§3–§6).

---

# 3. Product Line Overview

| Series code | Series name (CN) | Series name (EN, descriptive) | Sensing principle | Thickness range | Notes |
|---|---|---|---|---|---|
| **TSZ** | 一体超薄光编系列 | Integrated ultra-thin **optical** absolute encoder | Photoelectric (光电原理) | 10–16 mm | Two flange styles: Y-type and T-type |
| **SZM** | 一体超薄磁编系列 | Integrated ultra-thin **magnetic** absolute encoder | Magnetoelectric induction (磁电感应原理) | 12 mm (fixed) | Two flange styles: Y-type and T-type |
| **TSM** | 分体超薄磁编系列 | Split-body ultra-thin **magnetic** absolute encoder | Magnetoelectric induction (磁电感应原理) | 5.8–6.8 mm | Stator/rotor split design; resists demagnetization up to +150°C |
| **SE** | 通用绝对式系列 | General-purpose absolute encoder | Not stated (housed/shafted design) | N/A (conventional housed encoder) | Solid-shaft ("实心轴") and hollow-shaft ("空心轴") variants |

All four series share this electrical/interface envelope unless noted otherwise in their own section:
- Interfaces available: SSI, BiSS-C, CANopen, RS422, RS485 (SE series additionally offers TTL)
- Supply voltage: DC5V (SE series additionally offers DC24V)
- Standard operating temperature: −40°C ~ +65°C (see extended-grade caveat in Source-Fidelity Notes)

---

# 4. TSZ Series — Integrated Ultra-thin Optical Encoder (一体超薄光编系列)

## 4.1 Features
1. Ultra-thin: 10–16 mm
2. Photoelectric sensing principle; high positioning accuracy: ±5 arcsec
3. Integrated (one-piece) structure; simple installation; no secondary calibration needed
4. Dust-proof design
5. Redundant design with automatic error correction
6. Zero-position marking
7. Positioned as a domestic-manufacture replacement for imported brands (HEIDENHAIN, Renishaw, Netzer, etc.); custom domestic-substitution builds available

## 4.2 Electrical & Performance Specifications

| Model | OD / Shaft dia / Thickness (mm) | Weight (kg) | Resolution | Positioning accuracy | Repeatability | Supply voltage | Interface | Operating temp (°C) |
|---|---|---|---|---|---|---|---|---|
| TSZ35 | 35 / 6(7) / 10 | 0.024 | 22 bit | ±5″ | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSZ38 | 38 / 10 / 11 | 0.035 | 22 bit | ±5″ | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSZ58 | 58 / 25 / 12 | 0.073 | 22 bit | ±5″ | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSZ70 | 70 / 30 / 12 | 0.12 | 24 bit | ±5″ | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSZ85 | 85 / 40 / 12 | 0.15 | 24 bit | ±5″ | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSZ100 | 100 / 50 / 13 | 0.23 | 24 bit | ±5″ | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSZ130 | 130 / 90 / 16 | 0.44 | 25 bit | ±5″ | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |

Note: "6(7)" for TSZ35 shaft diameter indicates two selectable shaft-bore options (Ø6 mm or Ø7 mm).

## 4.3 Outline Drawing & Dimensions — Y-type (Y型, stepped/pedestal flange)

Drawing features (labeled): outer flange with d4 bolt holes (均布 = evenly spaced), zero-position mark (零位), d3 bolt-circle holes, cable Ø4 mm × 300 mm length (线缆Φ4 长300mm), elastic flange (弹性法兰).

| Model | D | d | H | d1 | d2 | d3 | h1 | d4 |
|---|---|---|---|---|---|---|---|---|
| TSZ35 | Φ35 | Φ6/7 | 10 | Φ2.8 | — | — | 31 | 2×Φ2.4 |
| TSZ38 | Φ38 | Φ10 | 10.5 | Φ3.5 | — | — | 34 | 2×Φ2.4 |
| TSZ58 | Φ58 | Φ25 | 12 | Φ17 | Φ21 | 4×Φ2.3 | 53 | 2×Φ2.8 |
| TSZ70 | Φ70 | Φ30 | 12 | Φ20 | Φ24.5 | 4×Φ2.8 | Φ75* | 3×Φ2.8 |
| TSZ85 | Φ85 | Φ40 | 12 | Φ30 | Φ35 | 4×Φ2.8 | 81 | 2×Φ2.8 |
| TSZ100 | Φ100 | Φ50 | 13 | Φ40 | Φ45 | 4×Φ2.8 | 96 | 2×Φ2.8 |
| TSZ130 | Φ130 | Φ90 | 16 | Φ78 | Φ84 | 6×Φ2.8 | Φ137* | 4×Φ3.5 |

\* Printed with a diameter symbol in the source table (see Source-Fidelity Notes) — verify against the actual drawing before use.
"—" = dash printed in source (dimension not applicable to this model / diagonal strike-through in original table).

## 4.4 Outline Drawing & Dimensions — T-type (T型, through-bore flange)

| Model | D | d | H | H1 | h1 | d4 |
|---|---|---|---|---|---|---|
| TSZ35 | Φ35 | Φ6/7 | 10 | 10.5 | 31 | 2×Φ2.4 |
| TSZ38 | Φ38 | Φ10 | 10.5 | 11 | 34 | 2×Φ2.4 |
| TSZ58 | Φ58 | Φ25 | 12 | 12.5 | 53 | 2×Φ2.8 |
| TSZ70 | Φ70 | Φ30 | 12 | 12.5 | Φ75* | 3×Φ2.8 |
| TSZ85 | Φ85 | Φ40 | 12 | 12.5 | 81 | 2×Φ2.8 |
| TSZ100 | Φ100 | Φ50 | 13 | 13.5 | 96 | 2×Φ2.8 |
| TSZ130 | Φ130 | Φ90 | 16 | 15 | Φ137* | 4×Φ3.5 |

\* See dimension note in §4.3.

Cable spec: Ø4 mm, 300 mm length, same as Y-type.

## 4.5 Ordering / Part Code (选型表)

Example code as printed: `TSZ 100 T50 T 22 1 T C B L X`

| Segment position | Example value | Field | Options |
|---|---|---|---|
| 1 | TSZ | Series | Fixed: TSZ |
| 2 | 100 | 外径 (Outer diameter, mm) | e.g., 35 / 38 / 58 / 70 / 85 / 100 / 130 |
| 3 | 50 | 轴径 (Shaft diameter, mm) | Model-dependent (see §4.2) |
| 3b | T | 安装形式 (Mounting style) | T = 通孔型 (through-bore / T-type); Y = 梯台型 (stepped/pedestal / Y-type) |
| 4 | 22 | 位数 (Resolution, bits) | 10–25 bit |
| 5 | 1 | 工作电压 (Supply voltage) | 1 = 5V |
| 6 | T | 接口 (Interface) | T: RS485; S: SSI; SC: SSI with checksum (SSI带校验); B: BiSS-C; M: Modbus; C: CANopen |
| 7 | C | 波特率/更新率 (Baud rate / update rate) | A: 2 MB/s–10 KHz; B: 921.6 KB/s–4 KHz; C: 115.2 KB/s–1 KHz; D: 57.6 KB/s–0.5 KHz; E: 9.6 KB/s–0.1 KHz |
| 8 | B | 码制 (Code type) | B: Binary (二进制); G: Gray code (格雷码) |
| 9 | L | 出线方式 (Cable exit style) | L: 散线侧出 (loose-wire, side exit) |
| 10 | X | 用户代码 (Customer code) | Customer-specific, open field |

**Note on segment 3:** in the printed example, the mounting-style letter (`T`) is written immediately before the shaft-diameter number (e.g., `T50`), even though the two are documented as separate fields (轴径 then 安装形式) in the row beneath. Treat a code like `T50` as "T-type mount, Ø50 mm shaft."

## 4.6 Installation Instructions (安装说明)

**Y-type product installation (Y型产品安装图)** — exploded view labels, top to bottom: encoder flange fixing screw (编码器法兰固定螺钉) → encoder shaft fixing screw (编码器轴固定螺钉) → encoder (编码器) → encoder flange positioning (编码器法兰定位) → shaft under test (被测轴).

**T-type product installation (T型产品安装图)** — exploded view labels: encoder flange fixing screw (编码器法兰固定螺钉) → encoder shaft lock nut (编码器轴锁紧螺母) → encoder (编码器) → encoder flange positioning (编码器法兰定位) → shaft under test (被测轴).

Steps (安装说明):
1. Align the encoder's main shaft to the shaft under test and gently push it in; use a matching screw or lock nut to fix the encoder's main shaft to the measured shaft (编码器主轴对准被测轴并轻轻推入，利用相匹配螺钉或锁紧螺母将编码器主轴与被测轴固定).
2. Use matching flange-mounting screws to secure the encoder flange at the flange locating position to complete installation (用相匹配的法兰固定螺钉将编码器法兰固定在法兰固定位处即可完成安装).

**Caution (注):** To prevent the fixing screw or lock nut from loosening and causing shift/slippage between the encoder shaft and the measured shaft, apply thread-locking adhesive to the screw/nut threads before final tightening (在紧固螺钉或锁紧螺母螺纹处涂抹螺纹胶后再旋紧).

Reference materials indicated on page (QR codes, not decodable from image — links not extractable): "PDF说明书" (PDF manual) and "STP链接" (STP/CAD model link).

---

# 5. SZM Series — Integrated Ultra-thin Magnetic Encoder (一体超薄磁编系列)

## 5.1 Features
1. Ultra-thin: 12 mm
2. Magnetoelectric induction principle; suitable for harsh operating conditions (适用于恶劣工况)
3. Integrated (one-piece) structure; simple installation; no secondary calibration needed
4. Positioning accuracy: ±0.01°
5. Redundant design, automatic error correction, strong anti-interference performance
6. Zero-position marking
7. Positioned as a domestic-manufacture replacement for imported brands (HEIDENHAIN, Renishaw, Netzer, etc.); custom domestic-substitution builds available

## 5.2 Electrical & Performance Specifications

| Model | OD / Shaft dia / Thickness (mm) | Weight (kg) | Resolution | Positioning accuracy | Repeatability | Supply voltage | Interface | Operating temp (°C) |
|---|---|---|---|---|---|---|---|---|
| SZM40 | 40 / 10 / 12 | 0.06 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| SZM58 | 58 / 20 / 12 | 0.08 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| SZM70 | 70 / 30 / 12 | 0.13 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| SZM85 | 85 / 40 / 12 | 0.15 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| SZM100 | 100 / 50 / 12 | 0.24 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |

## 5.3 Outline Drawing & Dimensions — Y-type (Y型)

| Model | D | d | H | h | d1 | d2 | d3 | h1 | d4 |
|---|---|---|---|---|---|---|---|---|---|
| SZM40 | Φ40 | Φ10 | 12 | 7 | Φ3.5 | — | — | 36 | 2×Φ2.4 |
| SZM58 | Φ58 | Φ20 | 12 | 7 | Φ12 | Φ16 | Φ2.3 (4×) | 53 | 2×Φ2.8 |
| SZM70 | Φ70 | Φ30 | 12 | 5.5 | Φ20 | Φ24.5 | Φ2.8 (4×) | Φ75* | 3×Φ2.8 |
| SZM85 | Φ85 | Φ40 | 12 | 7 | Φ30 | Φ35 | Φ2.8 (4×) | 81 | 2×Φ2.8 |
| SZM100 | Φ100 | Φ50 | 12 | 7 | Φ40 | Φ45 | Φ2.8 (4×) | 96 | 2×Φ2.8 |

\* See dimension note in §4.3. "—" = printed dash (not applicable for this model).
Cable: Ø4 mm, 300 mm length. d3 holes are evenly spaced per drawing annotation "4-d3 均布".

## 5.4 Outline Drawing & Dimensions — T-type (T型)

| Model | D | d | H | H1 | h1 | d4 |
|---|---|---|---|---|---|---|
| SZM40 | Φ40 | Φ10 | 12 | 12.5 | 36 | 2×Φ2.4 |
| SZM58 | Φ58 | Φ20 | 12 | 12.5 | 53 | 2×Φ2.8 |
| SZM70 | Φ70 | Φ30 | 12 | 12.5 | Φ75* | 3×Φ2.8 |
| SZM85 | Φ85 | Φ40 | 12 | 12.5 | 81 | 2×Φ2.8 |
| SZM100 | Φ100 | Φ50 | 12 | 12.5 | 96 | 2×Φ2.8 |

\* See dimension note in §4.3.

## 5.5 Ordering / Part Code (选型表)

Example code as printed: `SZM 85 T40 T 19 1 T C B T X`

| Segment position | Example value | Field | Options |
|---|---|---|---|
| 1 | SZM | Series | Fixed: SZM |
| 2 | 85 | 外径 (Outer diameter, mm) | 40 / 58 / 70 / 85 / 100 |
| 3 | 40 | 轴径 (Shaft diameter, mm) | Model-dependent (see §5.2) |
| 3b | T | 安装形式 (Mounting style) | T: 通孔型 (through-bore); Y: 梯台型 (stepped/pedestal) |
| 4 | 19 | 位数 (Resolution, bits) | 19 bit |
| 5 | 1 | 工作电压 (Supply voltage) | 1 = 5V |
| 6 | T | 接口 (Interface) | T: RS485; S: SSI; SC: SSI with checksum; B: BiSS-C; M: Modbus; C: CANopen |
| 7 | C | 波特率/更新率 (Baud rate / update rate) | A: 2 MB/s–10 KHz; B: 921.6 KB/s–4 KHz; C: 115.2 KB/s–1 KHz; D: 57.6 KB/s–0.5 KHz; E: 9.6 KB/s–0.1 KHz |
| 8 | B | 码制 (Code type) | B: Binary (二进制); G: Gray code (格雷码) |
| 9 | T | 出线方式 (Cable exit style) | T: 端子后出 (terminal, rear exit); G: 端子侧出 (terminal, side exit); Q: 散线后出 (loose-wire, rear exit); L: 散线侧出 (loose-wire, side exit) |
| 10 | X | 用户代码 (Customer code) | Customer-specific, open field |

Same segment-3 note as §4.5 applies (mount letter prefixed to shaft-diameter number, e.g. `T40`).

## 5.6 Installation Instructions

Same procedure and caution text as TSZ series — see §4.6 (identical Chinese text printed for both series' Y-type and T-type installation diagrams). QR codes for "PDF说明书" (PDF manual) and "STP链接" (STP/CAD model) also provided on this page but not decodable from the source image.

---

# 6. TSM Series — Split-body Ultra-thin Magnetic Encoder (分体超薄磁编系列)

## 6.1 Features
1. Ultra-thin: 5.8–6.8 mm
2. Magnetoelectric induction principle; suitable for harsh operating conditions
3. Positioning accuracy: ±0.01°
4. Configurable sensing magnetic flux; immune to motor interference (感应磁通量可设置，无惧电机干扰)
5. Resists demagnetization; rated to +150°C (不易退磁 (+150℃))

## 6.2 Electrical & Performance Specifications

| Model | OD / Shaft dia / Thickness (mm) | Weight (kg) | Resolution | Positioning accuracy | Repeatability | Supply voltage | Interface | Operating temp (°C) |
|---|---|---|---|---|---|---|---|---|
| TSM42T10T | 42 / 10 / 5.8 | 0.02 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSM48T15T | 48 / 15 / 5.8 | 0.028 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSM58T20T | 58 / 20 / 6.3 | 0.038 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSM70T30Y | 70 / 30 / 5.8 | 0.045 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSM80T40Y | 80 / 40 / 5.8 | 0.055 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |
| TSM100T50Y | 100 / 50 / 6.8 | 0.09 | 19 bit | ±0.01° | ±1″ | DC5V | SSI, BiSS-C, CANopen, RS422, RS485 | −40/−55 ~ +65 |

Note: model suffix letter (T/Y) appears to already encode a mounting variant per model, unlike TSZ/SZM where mount style is a separate ordering-code field.

## 6.3 Outline Drawing & Dimensions — Y-type (Y型尺寸表)

| Model | D | d | H | h | d1 | d2 | d3 | d4 | d5 |
|---|---|---|---|---|---|---|---|---|---|
| TSM42 | Φ42 | Φ10 | 5.8 | 2.2 | Φ37 | 3×Φ2.3 | — | — | Φ16 |
| TSM48 | Φ48 | Φ15 | 5.8 | 2.2 | Φ43 | 3×Φ2.3 | — | — | Φ20 |
| TSM58 | Φ58 | Φ20 | 6.3 | 2.2 | Φ53 | 3×Φ2.3 | — | — | Φ27 |
| TSM70 | Φ70 | Φ30 | 5.8 | 2.2 | Φ65 | 3×Φ2.3 | Φ34 | 4×Φ2.3 | Φ40 |
| TSM80 | Φ80 | Φ40 | 5.8 | 2.2 | Φ75 | 3×Φ2.3 | Φ44 | 4×Φ2.3 | Φ48 |
| TSM100 | Φ100 | Φ50 | 6.8 | 2.7 | Φ95 | 4×Φ2.3 | Φ55 | 4×Φ2.3 | Φ65 |

"—" = printed dash (not applicable for this model). Cable: Ø4 mm, 300 mm length.

## 6.4 Ordering / Part Code (选型表)

Example code as printed: `TSM 100 T50 T 19 1 T C B L X`

| Segment position | Example value | Field | Options |
|---|---|---|---|
| 1 | TSM | Series | Fixed: TSM |
| 2 | 100 | 外径 (Outer diameter, mm) | 42 / 48 / 58 / 70 / 80 / 100 |
| 3 | 50 | 轴径 (Shaft diameter, mm) | Model-dependent (see §6.2) |
| 3b | T | 安装形式 (Mounting style) | Fixed for this series: 分体安装 (split-body mounting) |
| 4 | 19 | 位数 (Resolution, bits) | 19 bit |
| 5 | 1 | 工作电压 (Supply voltage) | 1 = DC5V |
| 6 | T | 接口 (Interface) | T: RS485; S: SSI; SC: SSI with checksum; B: BiSS-C; M: Modbus; C: CANopen |
| 7 | C | 波特率/更新率 (Baud rate / update rate) | A: 2 MB/s–10 KHz; B: 921.6 KB/s–4 KHz; C: 115.2 KB/s–1 KHz; D: 57.6 KB/s–0.5 KHz; E: 9.6 KB/s–0.1 KHz |
| 8 | B | 码制 (Code type) | B: Binary (二进制); G: Gray code (格雷码) |
| 9 | L | 出线方式 (Cable exit style) | L: 散线侧出 (loose-wire, side exit) — only option printed |
| 10 | X | 用户代码 (Customer code) | Customer-specific, open field |

## 6.5 Installation Instructions

**Y-type installation diagram (Y型安装图)** — exploded view labels: stator fixing screw (定子固定螺钉) → encoder stator (编码器定子) → encoder magnetic ring fixing screw (编码器磁环紧固螺钉) → encoder (编码器) → shaft under test (被测轴) → stator fixing seat (定子固定座).

**T-type installation diagram (T型安装图)** — same label set as Y-type (定子固定螺钉, 编码器定子, then continuing off-crop but consistent with split-body magnetic-ring assembly).

Steps (安装说明):
1. Align the encoder's main shaft to the shaft under test and gently push it in; use a matching screw or lock nut to fix the encoder's main shaft to the measured shaft (编码器主轴对准被测轴并轻轻推入，利用相匹配螺钉或锁紧螺母将编码器主轴与被测轴固定).
2. Place the encoder stator into the stator fixing flange and secure with fixing screws to complete installation (将编码器定子放入定子固定法兰内，用螺钉紧固即可完成安装).

**Caution (注):** Measured-shaft tolerance h6; stator fixing flange positioning-bore tolerance H7; the encoder shaft end face and stator fixing flange mounting face height tolerance is ±0.1 mm (被测轴公差h6，定子固定法兰定位孔公差H7，被测轴端面与定子固定法兰安装面高度公差±0.1mm). To prevent the fixing screw or lock nut from loosening and causing shift/slippage between the encoder shaft and the measured shaft, apply thread-locking adhesive to the screw/nut threads before final tightening.

QR codes for "PDF说明书" (PDF manual) and "STP链接" (STP/CAD model) also provided on this page but not decodable from the source image.

---

# 7. SE Series — General-purpose Absolute Encoder (通用绝对式系列)

## 7.1 Features
1. Integrated (one-piece) structure; no secondary calibration needed
2. Built-in spring/elastic disc; simple installation (内置弹片、安装简易)
3. Multiple shaft-diameter options available (多种轴径可选)

## 7.2 Electrical & Performance Specifications

| Model | OD / Shaft dia / Thickness (mm) | Weight (kg) | Resolution | Positioning accuracy | Repeatability | Supply voltage | Interface | Operating temp (°C) |
|---|---|---|---|---|---|---|---|---|
| SE38S6 | 38 / 6 / 33.5 | 0.15 | 22 bit | ±20″ | ±1″ | DC5V, DC24V | SSI, BiSS-C, CANopen, RS422, RS485, TTL | −40/−55 ~ +65 |
| SE38T6 | 38 / 6 / 23.5 | 0.13 | 22 bit | ±20″ | ±1″ | DC5V, DC24V | SSI, BiSS-C, CANopen, RS422, RS485, TTL | −40/−55 ~ +65 |
| SE50T8 | 50 / 8 / 34.7 | 0.17 | 22 bit | ±20″ | ±1″ | DC5V, DC24V | SSI, BiSS-C, CANopen, RS422, RS485, TTL | −40/−55 ~ +65 |
| SE58T20 | 56 / 20 / 49 | 0.25 | 23 bit | ±15″ | ±1″ | DC5V, DC24V | SSI, BiSS-C, CANopen, RS422, RS485, TTL | −40/−55 ~ +65 |
| SE80T30 | 80 / 30 / 41.5 | 0.42 | 23 bit | ±10″ | ±1″ | DC5V, DC24V | SSI, BiSS-C, CANopen, RS422, RS485, TTL | −40/−55 ~ +65 |
| SE100T50 | 100 / 50 / 50.5 | 0.81 | 24 bit | ±5″ | ±1″ | DC5V, DC24V | SSI, BiSS-C, CANopen, RS422, RS485, TTL | −40/−55 ~ +65 |
| SE110T60 | 110 / 60 / 56 | 1.17 | 24 bit | ±5″ | ±1″ | DC5V, DC24V | SSI, BiSS-C, CANopen, RS422, RS485, TTL | −40/−55 ~ +65 |
| SE180T90 | 180 / 90 / 60 | 1.5 | 24 bit | ±2″ | ±1″ | DC5V, DC24V | SSI, BiSS-C, CANopen, RS422, RS485, TTL | −40/−55 ~ +65 |

Note: model naming convention appears to be `SE[OD][S or T][shaft-dia]` where **S = solid shaft (实心轴)** and **T = hollow/through shaft (空心轴)** — consistent with the two outline-drawing families below. Note SE58T20's printed outer diameter is 56 mm (not 58) — transcribed exactly as printed; likely a naming/spec inconsistency in the source.

## 7.3 Outline Drawing & Dimensions — Solid-shaft type (实心轴型尺寸表)

Only two models are documented with solid-shaft dimensions in the source (see Source-Fidelity Notes):

| Model | D | d | L | D1 | L1 | L2 | d1 | d2 |
|---|---|---|---|---|---|---|---|---|
| SE38S6 | Φ38 | Φ6 | 53.5 | Φ20 | 15 | 5 | Φ30 | 3×M3 |
| SE58S10 | Φ58 | Φ10 | 69 | Φ36 | 20 | 10 | Φ48 | 3×M4 |

Drawing shows a shafted-motor-style body: overall length L, shaft length L1/L2, front face bolt circle D1(h7)/d(h7), and d1/d2 = mounting bolt circle and evenly-spaced (均布) tapped holes.

## 7.4 Outline Drawing & Dimensions — Hollow-shaft type (空心轴型尺寸表)

| Model | D | d | L | D1 | L1 | L2 | L3 | d1 |
|---|---|---|---|---|---|---|---|---|
| SE38T6 | Φ38 | Φ6 | 23.5 | Φ23 | 0.3 | 54 | 46 | 2×Φ3.5 |
| SE50T8 | Φ50 | Φ8 | 34.7 | Φ24 | 0.3 | 66.5 | 57 | 2×Φ3.2 |
| SE58T20 | Φ56 | Φ20 | 48 | Φ36 | 0.3 | 73.5 | 65 | 2×Φ2.8 |
| SE80T30 | Φ80 | Φ30 | 41.5 | Φ50 | 8.2 | 96 | 88 | 2×Φ3.5 |
| SE100T50 | Φ100 | Φ50 | 49.5 | Φ70 | 8.2 | 117.5 | 109 | 2×Φ3.5 |
| SE110T60 | Φ110 | Φ60 | 57 | Φ82 | 4 | 128 | 120 | 2×Φ3.5 |
| SE180T90 | Φ180 | Φ90 | 60 | Φ116 | 5 | — | Φ172 | 4×Φ3.5 |

"—" = printed dash for SE180T90's L2 (not applicable / diagonal strike-through in original table); note Φ172 appears in the L3 position for this row as printed.

## 7.5 Ordering / Part Code (选型表)

Example code as printed: `SE 180 T90 T 22 1 T C B T X`

| Segment position | Example value | Field | Options |
|---|---|---|---|
| 1 | SE | Series | Fixed: SE |
| 2 | 180 | 外径 (Outer diameter, mm) | 38 / 50 / 56–58 / 80 / 100 / 110 / 180 |
| 3 | 90 | 轴径 (Shaft diameter, mm) | Model-dependent (see §7.2) |
| 3b | T | 安装形式 (Mounting style) | T: 锁紧环 (lock/clamping ring); Y: 螺钉紧固 (screw-fastened) |
| 4 | 22 | 位数 (Resolution, bits) | 10–24 bit |
| 5 | 1 | 工作电压 (Supply voltage) | 1: DC5V; 2: DC24V |
| 6 | T | 接口 (Interface) | T: RS485; S: SSI; B: BiSS-C; M: Modbus; C: CANopen |
| 7 | C | 波特率/更新率 (Baud rate / update rate) | A: 2 MB/s–10 KHz; B: 921.6 KB/s–4 KHz; C: 115.2 KB/s–1 KHz; D: 57.6 KB/s–0.5 KHz; E: 9.6 KB/s–0.1 KHz |
| 8 | B | 码制 (Code type) | B: Binary (二进制); G: Gray code (格雷码) |
| 9 | T | 出线方式 (Cable exit style) | T: 后出 (rear exit); G: 侧出 (side exit) |
| 10 | X | 用户代码 (Customer code) | Customer-specific, open field |

## 7.6 Installation Instructions

**Solid-shaft installation (实心轴安装图)** — labels: shaft under test (被测轴) → coupling (联轴器) → encoder fixing screw (编码器固定螺钉) → encoder fixing flange (编码器固定法兰) → encoder (编码器). Mounted via a bracket with the encoder body clamped and coupled to the test shaft through a flexible coupling.

Steps:
1. Use matching encoder-fixing screws to mount the encoder body onto the encoder fixing flange (用相匹配的编码器固定螺钉将编码器主体固定到编码器固定法兰上).
2. Use a coupling to connect and firmly secure the encoder shaft to the shaft under test (用联轴器将编码器主轴与被测轴连接并固定牢靠).

**Caution:** To prevent the fixing screw or set screw from loosening and causing shift/slippage between the encoder shaft and the measured shaft, apply thread-locking adhesive to the screw threads before final tightening (为避免紧固螺钉或顶丝松动引起编码器主轴与被测轴之间的位移和打滑，安装过程中需要在紧固螺钉或顶丝螺纹处涂抹螺纹胶后再旋紧).

**Hollow-shaft installation (空心轴安装图)** — labels: flange positioning (法兰固定位) → shaft under test (被测轴) → flange fixing screw (法兰固定螺钉) → encoder (编码器) → shaft-locking screw (锁紧螺钉) → shaft lock collar (轴锁紧环).

Steps:
1. Align the encoder's main shaft to the shaft under test and gently push it in until the encoder flange contacts the flange locating feature (编码器主轴对准被测轴，轻轻推入使编码器法兰与法兰固定位接触).
2. Use matching flange-mounting screws to fix the encoder flange at the flange fixing position (用相匹配的法兰固定螺钉将编码器法兰固定在法兰固定位处).
3. Rotate and tighten the lock collar/screw to secure the encoder main shaft to the measured shaft, completing installation (旋紧锁紧螺钉，将编码器主轴与被测轴锁固，完成安装).

**Caution:** Same thread-locking-adhesive precaution as above, applied to the lock-collar/screw threads (为避免紧固螺钉松动引起编码器主轴与被测轴之间的位移和打滑，安装过程中需要在紧固螺钉螺纹处涂抹螺纹胶后再旋紧).

QR codes for "PDF说明书" (PDF manual) and "STP链接" (STP/CAD model) also provided on this page but not decodable from the source image.

---

# 8. Cross-series Quick Reference

## 8.1 All models, flat table (for lookup/filtering by an agent)

| Series | Model | OD (mm) | Shaft dia (mm) | Thickness (mm) | Weight (kg) | Resolution | Positioning accuracy | Sensing principle | Body style |
|---|---|---|---|---|---|---|---|---|---|
| TSZ | TSZ35 | 35 | 6/7 | 10 | 0.024 | 22 bit | ±5″ | Optical | Integrated flange (Y/T) |
| TSZ | TSZ38 | 38 | 10 | 11 | 0.035 | 22 bit | ±5″ | Optical | Integrated flange (Y/T) |
| TSZ | TSZ58 | 58 | 25 | 12 | 0.073 | 22 bit | ±5″ | Optical | Integrated flange (Y/T) |
| TSZ | TSZ70 | 70 | 30 | 12 | 0.12 | 24 bit | ±5″ | Optical | Integrated flange (Y/T) |
| TSZ | TSZ85 | 85 | 40 | 12 | 0.15 | 24 bit | ±5″ | Optical | Integrated flange (Y/T) |
| TSZ | TSZ100 | 100 | 50 | 13 | 0.23 | 24 bit | ±5″ | Optical | Integrated flange (Y/T) |
| TSZ | TSZ130 | 130 | 90 | 16 | 0.44 | 25 bit | ±5″ | Optical | Integrated flange (Y/T) |
| SZM | SZM40 | 40 | 10 | 12 | 0.06 | 19 bit | ±0.01° | Magnetic | Integrated flange (Y/T) |
| SZM | SZM58 | 58 | 20 | 12 | 0.08 | 19 bit | ±0.01° | Magnetic | Integrated flange (Y/T) |
| SZM | SZM70 | 70 | 30 | 12 | 0.13 | 19 bit | ±0.01° | Magnetic | Integrated flange (Y/T) |
| SZM | SZM85 | 85 | 40 | 12 | 0.15 | 19 bit | ±0.01° | Magnetic | Integrated flange (Y/T) |
| SZM | SZM100 | 100 | 50 | 12 | 0.24 | 19 bit | ±0.01° | Magnetic | Integrated flange (Y/T) |
| TSM | TSM42T10T | 42 | 10 | 5.8 | 0.02 | 19 bit | ±0.01° | Magnetic | Split stator/rotor |
| TSM | TSM48T15T | 48 | 15 | 5.8 | 0.028 | 19 bit | ±0.01° | Magnetic | Split stator/rotor |
| TSM | TSM58T20T | 58 | 20 | 6.3 | 0.038 | 19 bit | ±0.01° | Magnetic | Split stator/rotor |
| TSM | TSM70T30Y | 70 | 30 | 5.8 | 0.045 | 19 bit | ±0.01° | Magnetic | Split stator/rotor |
| TSM | TSM80T40Y | 80 | 40 | 5.8 | 0.055 | 19 bit | ±0.01° | Magnetic | Split stator/rotor |
| TSM | TSM100T50Y | 100 | 50 | 6.8 | 0.09 | 19 bit | ±0.01° | Magnetic | Split stator/rotor |
| SE | SE38S6 | 38 | 6 | 33.5 | 0.15 | 22 bit | ±20″ | Not stated | Housed, solid shaft |
| SE | SE38T6 | 38 | 6 | 23.5 | 0.13 | 22 bit | ±20″ | Not stated | Housed, hollow shaft |
| SE | SE50T8 | 50 | 8 | 34.7 | 0.17 | 22 bit | ±20″ | Not stated | Housed, hollow shaft |
| SE | SE58T20 | 56 | 20 | 49 | 0.25 | 23 bit | ±15″ | Not stated | Housed, hollow shaft |
| SE | SE80T30 | 80 | 30 | 41.5 | 0.42 | 23 bit | ±10″ | Not stated | Housed, hollow shaft |
| SE | SE100T50 | 100 | 50 | 50.5 | 0.81 | 24 bit | ±5″ | Not stated | Housed, hollow shaft |
| SE | SE110T60 | 110 | 60 | 56 | 1.17 | 24 bit | ±5″ | Not stated | Housed, hollow shaft |
| SE | SE180T90 | 180 | 90 | 60 | 1.5 | 24 bit | ±2″ | Not stated | Housed, hollow shaft |
| SE | SE58S10 (dimensions only, see §7.3) | 58 | 10 | — | — | — | — | Not stated | Housed, solid shaft |

## 8.2 Common interface/protocol vocabulary across all series

| Code letter (in 接口 field) | Meaning |
|---|---|
| S | SSI |
| SC | SSI with checksum/verification (SSI带校验) |
| B | BiSS-C |
| M | Modbus |
| C | CANopen |
| T | RS485 (used as the field label prefix in ordering tables when RS485 is the chosen interface) |

| Code letter (in 波特率/更新率 field) | Baud rate / update rate |
|---|---|
| A | 2 MB/s – 10 KHz |
| B | 921.6 KB/s – 4 KHz |
| C | 115.2 KB/s – 1 KHz |
| D | 57.6 KB/s – 0.5 KHz |
| E | 9.6 KB/s – 0.1 KHz |

| Code letter (in 码制 field) | Code type |
|---|---|
| B | Binary (二进制) |
| G | Gray code (格雷码) |

## 8.3 Table of Contents as printed in source (目录) — for provenance/traceability

| Printed page # | Section |
|---|---|
| 09 | TSZ一体超薄光编系列 (TSZ series) — section header |
| 10 | 产品简介 (Product introduction) |
| 11 | 外形图 (Outline drawings) |
| 12 | 安装说明 (Installation instructions) |
| 13 | SZM一体磁编系列 (SZM series) — section header |
| 14 | 产品简介 |
| 15 | 外形图 |
| 16 | 安装说明 |
| 17 | TSM分体磁编系列 (TSM series) — section header |
| 18 | 产品简介 |
| 19 | 安装说明 |
| 20 | SE通用绝对式系列 (SE series) — section header |
| 21 | 产品简介 |
| 22 | 外形图 |
| 23 | 安装说明 |
| 24 | **联系方式 (Contact information) — NOT PRESENT IN SOURCE FILE** |

Note: the printed page numbers in the TOC do not exactly match the physical grouping of content observed while reading the file (e.g., product intro, outline drawing, and installation content for a series are often co-located on the same physical spread rather than split exactly per the TOC's page assignments). Section boundaries in this Markdown file follow the actual observed content groupings, not the literal TOC page numbers.

---

# 9. Open Items / Recommended Follow-ups for a Human Reviewer

1. **Missing contact information** — obtain the actual page 24 (联系方式) from the manufacturer directly; it is not recoverable from this file.
2. **QR codes** — each installation page includes two QR codes ("PDF说明书" linking to a fuller PDF manual, and "STP链接" linking to a downloadable STP/CAD model). These could not be decoded from a rasterized image; if the linked resources are needed, request the original PDF with clickable/scannable QR codes or ask the manufacturer for the URLs directly.
3. **SE58T20 naming/OD inconsistency** — model name implies OD 58 mm but the printed OD column value is 56 mm; confirm with manufacturer which is correct.
4. **SE38S6 / SE58S10 solid-shaft models** — SE58S10 has dimensional data but is absent from the master electrical spec table (§7.2); SE38S6 is the only solid-shaft model with full specs. Confirm whether other SE hollow-shaft models also come in solid-shaft variants.
5. **Φ-prefixed values in "should be plain length" columns** (TSZ70/TSZ130 h1; SZM70 h1; SE180T90 L3) — confirm against manufacturer CAD/STP files whether these are genuine diameters or transcription artifacts from the original design file.
6. **Duplicate pages** — the supplied PDF should ideally be re-exported by its source without the duplicate spreads and with the missing contact page restored.
