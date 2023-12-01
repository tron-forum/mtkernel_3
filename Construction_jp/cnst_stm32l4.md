# μT-Kernel3.0 STM32L4 IoT-Engine向け構築手順書 <!-- omit in toc -->
## Version.2.00.00 <!-- omit in toc -->
## 2023.12.01 <!-- omit in toc -->

- [1. 概要](#1-概要)
  - [1.1 目的](#11-目的)
  - [1.2 対象OSおよびハードウェア](#12-対象osおよびハードウェア)
  - [1.3 対象開発環境](#13-対象開発環境)
- [2. Cコンパイラ](#2-cコンパイラ)
  - [2.1 GCCバージョン](#21-gccバージョン)
  - [2.2 動作検証時のオプション](#22-動作検証時のオプション)
  - [2.3 インクルードパス](#23-インクルードパス)
  - [2.4 標準ライブラリ](#24-標準ライブラリ)
- [3. 開発環境と構築手順](#3-開発環境と構築手順)
  - [3.1 STM32CubeIDEを使用した構築手順](#31-stm32cubeideを使用した構築手順)
    - [3.1.1 STM32CudeIDEの準備](#311-stm32cudeideの準備)
    - [3.1.2 プロジェクトの作成](#312-プロジェクトの作成)
    - [3.1.3 プロジェクトのビルド](#313-プロジェクトのビルド)
  - [3.2 Makeを使用したビルド方法](#32-makeを使用したビルド方法)
    - [3.2.1 開発ソフトのインストール](#321-開発ソフトのインストール)
    - [3.2.2 ビルド環境の準備](#322-ビルド環境の準備)
    - [3.2.3 プロジェクトのビルド](#323-プロジェクトのビルド)
- [4. アプリケーションプログラム](#4-アプリケーションプログラム)
  - [4.1 アプリケーションプログラムの作成](#41-アプリケーションプログラムの作成)
  - [4.2 アプリケーションプログラムの例](#42-アプリケーションプログラムの例)
- [5. 実機でのプログラム実行](#5-実機でのプログラム実行)
  - [5.1 STM32CuleIDEによるプログラムの実行](#51-stm32culeideによるプログラムの実行)
- [更新履歴](#更新履歴)

# 1. 概要
## 1.1 目的
本書は、TRONフォーラムからソースコードが公開されているSTM32L4 IoT-Engine向けμT-Kernel3.0の開発環境の構築手順を記す。  
以降、本ソフトとは前述のμT-Kernel3.0のソースコードを示す。  

## 1.2 対象OSおよびハードウェア
本書は以下を対象とする。  

|分類|名称|備考|
|-|-|-|
|OS|μT-Kernel3.00|TRONフォーラム|
|実機|STM32L4 IoT-Engine|UCテクノロジー製|
|搭載マイコン|STM32L486VG|STマイクロエレクトロニクス製|

## 1.3 対象開発環境
本ソフトはC言語コンパイラとして、GCC(GNU Compiler)を前提とする。  
ただし、本ソフトはハードウェア依存部を除けば、標準のC言語で記述されており、他のC言語コンパイラへの移植も可能で可能である。  

# 2. Cコンパイラ
## 2.1 GCCバージョン
本ソフトの検証に用いたGCCのバージョンを以下に記す。  

`arm-none-eabi-gcc (xPack GNU Arm Embedded GCC x86_64) 12.3.1 20230626`

## 2.2 動作検証時のオプション
本ソフトの動作検証時のコンパイラ及びリンカのオプションを示す。なお、オプションは、開発するアプリケーションに応じて適したものを指定する必要がある。  

最適化オプションは、検証時には-O2を設定している。  
リンクタイム最適化-flto( Link-time optimizer)については動作を保証しない。  

その他の主なオプションを以下に示す。  

コンパイルオプション  
`-mcpu=cortex-m4 -mthumb -ffreestanding -std=gnu11`
リンクオプション  
`-mcpu=cortex-m4 -mthumb -ffreestanding -nostartfiles`

## 2.3 インクルードパス
μT-Kernel3.0のソースディレクトリ中の以下のディレクトリを、ビルド時のインクルードパスに指定する。  

|ディレクトリパス|内容|
|-|-|
|config|コンフィギュレーションファイル|
|include|共通ヘッダファイル|
|kernel\knlinc|カーネル内共通ヘッダファイル|

kermel\knlincはOS内部でのみ使用するヘッダファイルである。ユーザプログラムでは、config とincludeのヘッダファイルのみを使用する。

## 2.4 標準ライブラリ
本ソフトは基本的にはコンパイラの標準ライブラリを使用しない。ただし、演算に際してライブラリが使用される場合がある。本ソフトではデバッグサポート機能の中の演算で使用されている（td_get_otmおよびtd_get_timの処理内で__aeabi_idivmod関数が使用されている）。  
デバッグサポート機能を使用しない場合は、標準ライブラリは不要である。リンカオプションで-nostdlibが指定可能となる。ただし、アプリケーションで使用している場合はこの限りではない。  

# 3. 開発環境と構築手順
本ソフトをビルドするための開発環境の準備と構築手順を説明する。  

本ソフトは極力、特定の開発環境に依存しないように作られている。ここでは例として、WindowsのPCにおいて、統合開発環境STM32CCudeIDEを使用する場合と、自動ビルドツールMakeを使用する場合を説明する。  
なお、ここに示す開発環境や構築手順はあくまで例であり、ユーザそれぞれの環境などによって差異がある場合がある。  

## 3.1 STM32CubeIDEを使用した構築手順
### 3.1.1 STM32CudeIDEの準備
STM32CudeIDEは、オープンソースの"Eclipse"をベースとした、STマイクロエレクトロニクス製マイコン用の統合開発環境である。  
以下から使用するPCのOSに合わせて、STM32CudeIDEをダウンロードする。  

本ソフトの動作検証にはSTM32CudeIDEの以下のバージョンを使用した。  
`STM32CudeIDE v.1.13.2`  

STM32CudeIDEのダウンロードページ  
https://www.st.com/ja/development-tools/stm32cubeide.html

インストーラがダウンロードされるので、それを実行し、指示に従ってインストールを進める。  
STM32CudeIDEのインストールや操作については、上記のホームページを参照のこと。  

### 3.1.2 プロジェクトの作成
STM32CudeIDEにて以下の手順で本ソフトのプロジェクトを作成する。  

(1) STM32CudeIDEの初回起動時、指示に従いワークスペースを作成する。  
ワークスペースは、STM32CudeIDEの各種設定などが保存される可能的な作業場である。

(2) メニュー「New」→「C/C++ Project」を選択する。  
開いた新規C/C++プロジェクトのテンプレート画面で「C Managed build」を選択する。次のCプロジェクト画面で以下を設定する。
 - Project name：任意
 - Location：任意
 - Project type：「Empty Project」選択
 - Toolchains：「MCU ARM GCC」選択

「Next」ボタンを押下し進め、MCUには「STM32L486VGTx」を選択し、最後に「Finish
」ボタンを押下するとプロジェクトが生成される。  


(3) 作成したプロジェクトを選択した状態でメニュー「File」→「Import...」を選択する。  
開いた選択画面で「General」→「File System」を選択し、ファイルシステム画面でμT-Kernel3.0のソースコードのディレクトリを入力する。  
なお、(1)でプロジェクトのロケーションに、既にソースコードのディレクトリが存在するディレクトリを指定した場合は、インポートは不要である。  

(4) 作成したプロジェクトを選択した状態でメニュー「Project」→「Properties」を選択する。  
以降、プロパティのダイアログにて各項目を設定していく。なお、本書の設定は一例であり、必要に応じて変更すること。  

(5) ダイアログの項目「C/C++ Build」→「Settings」を選択し、「Tool Setteings」タブを開いて以下の設定を行う。  

|||
|-|-|
|MCU Settings」|「MCU」：「STM32L486VGTx」を確認<br>「Floating-point unit」：任意（使用しない場合は「None」）|
|MCU GCC Assembler|「Preprocessor」：<br>「Dedine symbols (-D)」：「\_IOTE_ST32L4\_」<br>「Include paths」」：μT-Kernel3.0のインクルードパスを設定|
|MCU GCC Compiler」|「General」：<br>　　「Language standard」：「GNU11 (-std=gnu11)」<br>　　「Preprocessor」：<br>　　「Define symbols (-D)」：「\_IOTE_STM32L4\_」<br>「Include paths」：<br>　　「Include paths」：μT-Kernel3.0のインクルードパスを設定<br>　　「Optimization」：「Optimaizationlevel」：任意<br>　　他はチェックせず|
| MCU GCC Linker|「General」：<br>　　「Linker Script (-T)」：以下のスクリプト・ファイルを指定<br>　　mtkernel_3\etc\linker\iote_stm32l4\tkernel_map.ld<br>　　「Do not use standard start files (-nostartfiles)」をチェック|

すべての設定を終えたら「Apply and Close」ボタンを押下し設定を終える。

### 3.1.3 プロジェクトのビルド
作成したプロジェクトを選択した状態でメニュー「Projects」→「Build Project」を選択すると、μT-Kernelのソースコードがコンパイル、リンクされ、実行コードのELFファイルが生成される。

## 3.2 Makeを使用したビルド方法
### 3.2.1 開発ソフトのインストール
Cコンパイラなど共通の開発ツールをインストールする。

(1) Cコンパイラのインストール
   GCCコンパイラ一式を以下から入手し、Webページの指示に従いインストールする。
   
The xPack GNU Arm Embedded GCC  
https://xpack.github.io/arm-none-eabi-gcc/  

本稿作成時に検証したバージョンは以下の通り。

`arm-none-eabi-gcc (xPack GNU Arm Embedded GCC x86_64) 12.3.1 20230626`

(2) 開発ツールのインストール  
開発ツール一式（makeなど）を以下から入手し、Webページの指示に従いインストールする。
   
xPack Windows Build Tools  
https://xpack.github.io/windows-build-tools/  

本稿作成時に検証したバージョンは以下の通り。  
`xPack Windows Build Tools v4.4.1-2`
 
(3) 実行パスの設定  
Windowsのコマンドシェル（PowerShellまたはコマンドプロンプト）から、GCCおよびMakeが実行可能となるように、環境変数pathにGCCを展開したディレクトリ内の\binディレクトリのパスおよび、xPack Windows Build Toolsを展開したディレクトリ内の\binディレクトリのパスを追加設定する。  
コマンドシェルからGCC（arm-none-eabi-gcc）およびmakeコマンドが実行可能であることを確認する。  

### 3.2.2 ビルド環境の準備
(1) makefileの設定  
本ソフトのソースコード中のMake用ビルドディレクトリ(build_make)にmakefileが格納されている。  
ディレクトリ(build_make)の内容を以下に示す。   

|名称|説明|
|-|-|
|makefile|μT-Kernel 3.0のビルド規則（ルート）|
|iote_m367.mk|M367 IoT-Engine用のビルド規則(※)|
|iote_rx231.mk|RX231 IoT-Engine用のビルド規則(※)|
|iote_stm32l4.mk|STM32L4 IoT-Engine用のビルド規則|
/mtkernel_3|Make作業用ディレクトリ|

※ 本書の説明では使用しない。

makefileファイルの先頭の以下の定義を変更する。  

|定義名|初期値|説明|
|-|-|-|
|EXE_FILE|mtkernel_3|ビルドする実行ファイル名|
|TARGET|\_IOTE_STM32L4\_|対象とするハードウェア<br>STM32L4 IoT-Engineの場合は「\_IOTE_STM32L4\_」に変更する|

また、iote_stm32l4.mkの先頭の以下の定義を必要に応じて変更する。  

|定義名|初期値|説明|
|-|-|-|
|GCC|arm-none-eabi-gcc|Cコンパイラのコマンド名|
|AS|arm-none-eabi-gcc|アセンブラのコマンド名|
|LINK|arm-none-eabi-gcc|リンカのコマンド名|
|CFLAGS|省略(※)|Cコンパイラのオプション|
|ASFLAGS|省略(※)|アセンブラのオプション|
|LFLAGS|省略(※)|リンカのオプション|
|LINKFILE|省略(※)|リンク定義ファイル|

※ iote_stm32l4.mkファイルの記述を参照

他のファイルについてはOSのソースコードの変更が無い限り、変更する必要はない。  
ただし、ユーザプログラムの追加等については、それぞれ対応するビルド規則を記述する必要がある。  

またapp_sampleディレクトリ下のアプリケーションについては以下のファイルでビルド規則が記述されている。  

`build_make\mtkernel_3\app_sample\subdir.mk`

app_sampleディレクトリにソースファイルを追加しても対応可能なビルド規則となっているが、サブディレクトリには対応してない。サブディレクトリを作成する場合はビルド規則の記述を変更する必要がある。  

### 3.2.3 プロジェクトのビルド
Windowsのシェル（PowerShellまたはコマンドプロンプト）上で、build_makeディレクトリをカレントディレクトリとし、以下のコマンドを実行する。  

`make all`

ビルドが成功すると、build_makeディレクトリ下に、実行コードのELFファイルが生成される。ELFファイルの名称はEXE_FILEで指定した名称である(初期値ではmtkernel_3.elfが生成される)。  
また、以下のコマンドを実行すると、ELFファイルおよびその他の中間生成ファイルが削除される。
`make clean`

# 4. アプリケーションプログラム
## 4.1 アプリケーションプログラムの作成
アプリケーションプログラムは、OSとは別にアプリ用のディレクトリを作成して、そこにソースコードを置き、OSと一括でコンパイル、リンクを行う。
アプリケーションプログラムには、usermain関数を定義する。OSは起動後に初期タスクからusermain関数を実行する。詳細はμT-Kernel3.0共通実装仕様書「5.2.3 ユーザ定義メイン関数usermain」を参照のこと。

アプリケーションからOSの機能を使用する場合は、以下のようにヘッダファイルのインクルードを行う。

`#include <tk/tkernel.h>`

T-Monitor互換ライブラリを使用する場合は、さらに以下のインクルードが必要である。

`#include <tm/tmonitor.h>`

μT-Kernel3.0の機能については、μT-Kernel3.0仕様書を参照のこと。

## 4.2 アプリケーションプログラムの例
アプリケーションプログラムの例を示す。このプログラムは、初期タスクでOSのバージョンをT-Monitor互換ライブラリを使用してシリアル出力にメッセージを出力したのち、二つのタスクtask_1とtask_2を生成、実行する。二つのタスクは同期をとり、1秒間隔でシリアル出力にメッセージを出力する。

```
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

/* task_1の定義 */
LOCAL void task_1(INT stacd, void *exinf);	// 実行関数
LOCAL ID	tskid_1;			// ID番号
LOCAL T_CTSK ctsk_1 = {
	.itskpri	= 10,			// 初期優先度
	.stksz		= 1024,			// スタックサイズ
	.task		= task_1,		// 実行関数のポインタ
	.tskatr		= TA_HLNG | TA_RNG3,	// タスク属性
};

/* task_2の定義 */
LOCAL void task_2(INT stacd, void *exinf);	// 実行関数
LOCAL ID	tskid_2;			// ID番号
LOCAL T_CTSK ctsk_2 = {
	.itskpri	= 10,			// 初期優先度
	.stksz		= 1024,			// スタックサイズ
	.task		= task_2,		// 実行関数のポインタ
	.tskatr		= TA_HLNG | TA_RNG3,	// タスク属性
};

/* task_1の実行関数 */
LOCAL void task_1(INT stacd, void *exinf)
{
	while(1) {
		tm_printf((UB*)"task 1\n");
		tk_wup_tsk(tskid_2);
		tk_dly_tsk(1000);
	}

}

/* task_2の実行関数 */
LOCAL void task_2(INT stacd, void *exinf)
{
	while(1) {
		tk_slp_tsk(TMO_FEVR);
		tm_printf((UB*)"task 2\n");
	}
}

/* usermain関数(初期タスクで実行) */
EXPORT INT usermain(void)
{
	T_RVER	rver;

	tm_putstring((UB*)"Start User-main program.\n");

	/* OSのバージョン表示 */
	tk_ref_ver(&rver);		/* OSバージョンの取得API */
	tm_printf((UB*)"Make Code: %04x  Product ID: %04x\n", rver.maker, rver.prid);
	tm_printf((UB*)"Product Ver. %04x\nProduct Num. %04x %04x %04x %04x\n", 
			rver.prver, rver.prno[0],rver.prno[1],rver.prno[2],rver.prno[3]);


	tskid_1 = tk_cre_tsk(&ctsk_1);		// タスクの生成
	tk_sta_tsk(tskid_1, 0);			// タスクの実行

	tskid_2 = tk_cre_tsk(&ctsk_2);		// タスクの生成
	tk_sta_tsk(tskid_2, 0);			// タスクの実行
	
	tk_slp_tsk(TMO_FEVR);

	return 0;
}
```

# 5. 実機でのプログラム実行
プログラムを実機上で実行する方法を、STM32CuleIDEとJTAGエミュレータ J-Link（Segger Microcontroller Systems製）を使用した例で説明する。  
STM32CuleIDEの開発環境からJ-Linkを使用し、実機に実行コードを転送しデバッグを行う。実機にはJ-Linkと接続するためのJTAGインタフェースが必要となる。  

## 5.1 STM32CuleIDEによるプログラムの実行
(1) STM32CuleIDEのメニューからメニュー「Run」→「Debug configurations」を選択し、開いたダイアログから項目「STM32 Cortex-M C/C++ Application」を選択する。

(2) 「New」ボタンを押し構成を追加する。

(3) 追加した構成を選択し、「構成の作成、管理、実行」画面にて以下の設定を行う。  

|||
|-|-|
|「Main」タブ|Name：任意<br>Project：前項で作成したプロジェクトを指定<br>C/C++ Application：ビルドしたELFファイル|
|「デバッガー」タブ|デバッグプローブ：「SEGGER J-LINK」を選択<br>Device：「STM32L486VG」を入力|
|「Startup」タブ|Set breakpoint at: 「usermain」を入力|

(4) デバッグ開始
「Debig」ボタンを押すとプログラムが実機に転送され、ROMに書き込まれたのち、実行される。  
プログラムは実行すると、OS起動後にユーザのアプリケーションプログラムを実行し、usermain関数にてブレークする。  


# 更新履歴

|版数|日付|内　容|
|-|-|-|
|2.00.00|2023.12.01|開発環境のバージョンの更新<br>ドキュメントフォーマットの変更<br>全体の見直しおよび変更|
|1.00.02|2022.06.30|開発環境のバージョンの更新<br>Eclipseベースの開発環境をPreiades All in One）からSTM32CubeIDEに変更|
|1.00.01|2021.05.17|開発環境のバージョンの更新<br>全体の見直しおよび変更|
|1.00.00|2020.12.09|初版|