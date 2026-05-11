# make/ — Makefile Build Configuration

This directory contains board-specific configuration files and common build
rules for the **Makefile-based** build system (an alternative to CMake).

## Structure

```
make/
├── rules.mk          # Common compile/assemble pattern rules
├── iote_m367.mk      # IoT-Engine M367     (Cortex-M3 / ARMv7-M)
├── iote_rx231.mk     # IoT-Engine RX231    (RX / RXv2)
├── iote_rza2m.mk     # IoT-Engine RZA2M    (Cortex-A9 / ARMv7-A)
└── iote_stm32l4.mk   # IoT-Engine STM32L4  (Cortex-M4 / ARMv7-M)
```

## How it works

The top-level `Makefile` selects a board via the `BOARD` variable:

```sh
make BOARD=iote_stm32l4
```

This includes `make/<BOARD>.mk`, which sets:

| Variable    | Description                                    | Example            |
|-------------|------------------------------------------------|--------------------|
| `GCC`       | C compiler                                     | `arm-none-eabi-gcc`|
| `AS`        | Assembler                                      | `arm-none-eabi-gcc`|
| `LINK`      | Linker                                         | `arm-none-eabi-gcc`|
| `CFLAGS`    | C compiler flags                               | `-mcpu=cortex-m4`  |
| `ASFLAGS`   | Assembler flags                                | `-mcpu=cortex-m4`  |
| `LFLAGS`    | Linker flags                                   | `-nostartfiles`    |
| `LNKFILE`   | Path to linker script                          | `etc/linker/...`   |
| `BOARD_DIR` | Board-level sysdepend directory                | `iote_stm32l4`     |
| `CPU_DIR`   | CPU-level sysdepend directory                  | `stm32l4`          |
| `CORE_DIR`  | CPU core architecture directory                | `armv7m`           |

These directory variables map to the µT-Kernel sysdepend layer hierarchy
(same as the CMake build — see `cmake/README.md` for details).

## rules.mk

`rules.mk` provides the common compile and assemble pattern rules used by
all sub-Makefiles. Each sub-Makefile sets `SRC_DIR` (relative to the kernel
root) and then includes `rules.mk` to get automatic source collection and
object file generation.

## Relationship with CMake

Both build systems (`Makefile` and `CMakeLists.txt`) coexist. They share:
- The same source tree
- The same sysdepend directory naming convention (`BOARD_DIR` / `CPU_DIR` / `CORE_DIR`)
- The same linker scripts under `etc/linker/`

When adding or modifying a board, update **both** `make/<board>.mk` and
`cmake/board/<board>.cmake` to keep them in sync.

---

# make/ — Makefile ビルド設定

このディレクトリには、**Makefile ベース**のビルドシステム (CMake の代替) で使用する
ボード固有の設定ファイルと共通ビルドルールが含まれています。

## 構成

```
make/
├── rules.mk          # 共通コンパイル/アセンブルパターンルール
├── iote_m367.mk      # IoT-Engine M367     (Cortex-M3 / ARMv7-M)
├── iote_rx231.mk     # IoT-Engine RX231    (RX / RXv2)
├── iote_rza2m.mk     # IoT-Engine RZA2M    (Cortex-A9 / ARMv7-A)
└── iote_stm32l4.mk   # IoT-Engine STM32L4  (Cortex-M4 / ARMv7-M)
```

## 仕組み

トップレベルの `Makefile` で `BOARD` 変数を指定してボードを選択します:

```sh
make BOARD=iote_stm32l4
```

これにより `make/<BOARD>.mk` が読み込まれ、以下の変数が設定されます:

| 変数        | 説明                                     | 例                 |
|-------------|------------------------------------------|--------------------|
| `GCC`       | C コンパイラ                             | `arm-none-eabi-gcc`|
| `AS`        | アセンブラ                               | `arm-none-eabi-gcc`|
| `LINK`      | リンカ                                   | `arm-none-eabi-gcc`|
| `CFLAGS`    | C コンパイラフラグ                       | `-mcpu=cortex-m4`  |
| `ASFLAGS`   | アセンブラフラグ                         | `-mcpu=cortex-m4`  |
| `LFLAGS`    | リンカフラグ                             | `-nostartfiles`    |
| `LNKFILE`   | リンカスクリプトへのパス                 | `etc/linker/...`   |
| `BOARD_DIR` | ボードレベルの sysdepend ディレクトリ    | `iote_stm32l4`     |
| `CPU_DIR`   | CPU レベルの sysdepend ディレクトリ      | `stm32l4`          |
| `CORE_DIR`  | CPU コアアーキテクチャディレクトリ       | `armv7m`           |

これらのディレクトリ変数は µT-Kernel の sysdepend レイヤ階層に対応しています
(CMake ビルドと同じ — 詳細は `cmake/README.md` を参照)。

## rules.mk

`rules.mk` は全サブ Makefile で使用される共通コンパイル・アセンブルパターンルールを
提供します。各サブ Makefile は `SRC_DIR` (カーネルルートからの相対パス) を設定した後
`rules.mk` を include することで、ソースの自動収集とオブジェクトファイル生成が行われます。

## CMake との関係

両ビルドシステム (`Makefile` と `CMakeLists.txt`) は共存しています。共有する要素:
- 同一のソースツリー
- 同一の sysdepend ディレクトリ命名規則 (`BOARD_DIR` / `CPU_DIR` / `CORE_DIR`)
- `etc/linker/` 配下の同一リンカスクリプト

ボードの追加・変更時は `make/<ボード名>.mk` と `cmake/board/<ボード名>.cmake` の
**両方**を更新してください。
