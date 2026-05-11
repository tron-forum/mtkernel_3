# cmake/ — CMake Build System

This directory contains the CMake toolchain and board definition files used
by the top-level `CMakeLists.txt`.

## Structure

```
cmake/
├── arm-none-eabi.cmake   # Toolchain file for ARM Cortex-M/A (GCC)
├── rx-elf.cmake          # Toolchain file for Renesas RX (GCC)
└── board/
    ├── iote_m367.cmake   # IoT-Engine M367     (Cortex-M3 / ARMv7-M)
    ├── iote_rx231.cmake  # IoT-Engine RX231    (RX / RXv2)
    ├── iote_rza2m.cmake  # IoT-Engine RZA2M    (Cortex-A9 / ARMv7-A)
    └── iote_stm32l4.cmake # IoT-Engine STM32L4 (Cortex-M4 / ARMv7-M)
```

## How it works

The top-level `CMakeLists.txt` selects a board via `-DBOARD=<name>`:

```sh
cmake -B build/iote_stm32l4 -DBOARD=iote_stm32l4
```

This includes `cmake/board/<BOARD>.cmake`, which sets:

| Variable          | Description                                          | Example            |
|-------------------|------------------------------------------------------|--------------------|
| `BOARD_DIR`       | Board-level sysdepend directory name                 | `iote_stm32l4`     |
| `CPU_DIR`         | CPU-level sysdepend directory name                   | `stm32l4`          |
| `CORE_DIR`        | CPU core architecture directory name                 | `armv7m`           |
| `BOARD_DEF`       | Preprocessor define for the target board             | `_IOTE_STM32L4_`   |
| `BOARD_C_FLAGS`   | Board-specific C compiler flags                      | `-mcpu=cortex-m4`  |
| `BOARD_ASM_FLAGS` | Board-specific assembler flags                       | `-mcpu=cortex-m4`  |
| `BOARD_LINK_FLAGS`| Board-specific linker flags                          | `-nostartfiles`    |
| `LINKER_SCRIPT`   | Path to linker script in `etc/linker/`               |                    |

These three directory variables (`BOARD_DIR`, `CPU_DIR`, `CORE_DIR`) map to the
µT-Kernel sysdepend layer hierarchy:

```
kernel/sysdepend/${BOARD_DIR}/       # Board-level (clock, HW init)
kernel/sysdepend/cpu/${CPU_DIR}/     # CPU-level (vector table)
kernel/sysdepend/cpu/core/${CORE_DIR}/ # Core-level (dispatch, interrupts)
```

Each board file also includes the appropriate toolchain file
(`arm-none-eabi.cmake` or `rx-elf.cmake`).

## Adding a new board

1. Create `cmake/board/<board_name>.cmake`
2. Set `BOARD_DIR`, `CPU_DIR`, `CORE_DIR`, `BOARD_DEF`, flags, and `LINKER_SCRIPT`
3. Include the matching toolchain file
4. Add a linker script under `etc/linker/<board_name>/`
5. Build: `cmake -B build/<board_name> -DBOARD=<board_name>`

---

# cmake/ — CMake ビルドシステム

このディレクトリには、トップレベルの `CMakeLists.txt` が使用する CMake ツールチェイン
ファイルとボード定義ファイルが含まれています。

## 構成

```
cmake/
├── arm-none-eabi.cmake   # ARM Cortex-M/A 用ツールチェインファイル (GCC)
├── rx-elf.cmake          # Renesas RX 用ツールチェインファイル (GCC)
└── board/
    ├── iote_m367.cmake   # IoT-Engine M367     (Cortex-M3 / ARMv7-M)
    ├── iote_rx231.cmake  # IoT-Engine RX231    (RX / RXv2)
    ├── iote_rza2m.cmake  # IoT-Engine RZA2M    (Cortex-A9 / ARMv7-A)
    └── iote_stm32l4.cmake # IoT-Engine STM32L4 (Cortex-M4 / ARMv7-M)
```

## 仕組み

トップレベルの `CMakeLists.txt` で `-DBOARD=<名前>` を指定してボードを選択します:

```sh
cmake -B build/iote_stm32l4 -DBOARD=iote_stm32l4
```

これにより `cmake/board/<BOARD>.cmake` が読み込まれ、以下の変数が設定されます:

| 変数              | 説明                                           | 例                 |
|-------------------|------------------------------------------------|--------------------|
| `BOARD_DIR`       | ボードレベルの sysdepend ディレクトリ名         | `iote_stm32l4`     |
| `CPU_DIR`         | CPU レベルの sysdepend ディレクトリ名           | `stm32l4`          |
| `CORE_DIR`        | CPU コアアーキテクチャのディレクトリ名          | `armv7m`           |
| `BOARD_DEF`       | ターゲットボード用プリプロセッサ定義            | `_IOTE_STM32L4_`   |
| `BOARD_C_FLAGS`   | ボード固有の C コンパイラフラグ                 | `-mcpu=cortex-m4`  |
| `BOARD_ASM_FLAGS` | ボード固有のアセンブラフラグ                    | `-mcpu=cortex-m4`  |
| `BOARD_LINK_FLAGS`| ボード固有のリンカフラグ                        | `-nostartfiles`    |
| `LINKER_SCRIPT`   | `etc/linker/` 内のリンカスクリプトへのパス      |                    |

3 つのディレクトリ変数 (`BOARD_DIR`, `CPU_DIR`, `CORE_DIR`) は µT-Kernel の
sysdepend レイヤ階層に対応しています:

```
kernel/sysdepend/${BOARD_DIR}/         # ボードレベル (クロック、HW 初期化)
kernel/sysdepend/cpu/${CPU_DIR}/       # CPU レベル (ベクタテーブル)
kernel/sysdepend/cpu/core/${CORE_DIR}/ # コアレベル (ディスパッチ、割込み)
```

各ボードファイルは対応するツールチェインファイル
(`arm-none-eabi.cmake` または `rx-elf.cmake`) を include しています。

## 新規ボードの追加

1. `cmake/board/<ボード名>.cmake` を作成
2. `BOARD_DIR`, `CPU_DIR`, `CORE_DIR`, `BOARD_DEF`, フラグ, `LINKER_SCRIPT` を設定
3. 対応するツールチェインファイルを include
4. `etc/linker/<ボード名>/` にリンカスクリプトを追加
5. ビルド: `cmake -B build/<ボード名> -DBOARD=<ボード名>`
