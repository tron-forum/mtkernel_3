# μT-Kernel3.0 RX231 IoT-Engine向け構築手順書 <!-- omit in toc -->
## Version.02.00.00 <!-- omit in toc -->
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
	- [3.1 e2 studioを使用した構築手順](#31-e2-studioを使用した構築手順)
		- [3.1.1 e2 studioの準備](#311-e2-studioの準備)
		- [3.1.2 プロジェクトの作成](#312-プロジェクトの作成)
		- [3.1.3 プロジェクトのビルド](#313-プロジェクトのビルド)
	- [3.2 Makeを使用したビルド方法](#32-makeを使用したビルド方法)
		- [3.2.1 開発ソフトの準備](#321-開発ソフトの準備)
		- [3.2.2 ビルド環境の準備](#322-ビルド環境の準備)
		- [3.2.3 プロジェクトのビルド](#323-プロジェクトのビルド)
- [4. アプリケーションプログラム](#4-アプリケーションプログラム)
	- [4.1 アプリケーションプログラムの作成](#41-アプリケーションプログラムの作成)
	- [4.2 アプリケーションプログラムの例](#42-アプリケーションプログラムの例)
- [5. 実機でのプログラム実行](#5-実機でのプログラム実行)
	- [5.1 E2 studioによるプログラムの実行](#51-e2-studioによるプログラムの実行)
- [更新履歴](#更新履歴)

# 1. 概要
## 1.1 目的
本書は、TRONフォーラムからソースコードが公開されているRX231 IoT-Engine向けμT-Kernel3.0の開発環境の構築手順を記す。  
以降、本ソフトとは前述のμT-Kernel3.0のソースコードを示す。

## 1.2 対象OSおよびハードウェア
本書は以下を対象とする。

|分類|名称|備考|
|-|-|-|
|OS|μT-Kernel3.00|TRONフォーラム|
|実機|RX231 IoT-Engine|UCテクノロジー製|
|搭載マイコン|RX200シリーズ RX231グループ<br>R5F52318ADFL|ルネサス エレクトロニクス製|

## 1.3 対象開発環境
本ソフトはC言語コンパイラとして、GCC(GNU Compiler)を前提とする。  
ただし、本ソフトはハードウェア依存部を除けば、標準のC言語で記述されており、他のC言語コンパイラへの移植も可能で可能である。

# 2. Cコンパイラ
## 2.1 GCCバージョン
本ソフトの検証に用いたGCCのツールチェーンを以下に記す。

`rx-elf-gcc.exe (GCC_Build_20230525) 8.3.0.202305-GNURX 20190222`

## 2.2 動作検証時のオプション
本ソフトの動作検証時のコンパイラ及びリンカのオプションを示す。なお、オプションは、開発するアプリケーションに応じて適したものを指定する必要がある。

最適化オプションは、-O2を設定している。  
リンクタイム最適化-flto( Link-time optimizer)については動作を保証しない。

その他の主なオプションを以下に示す。

コンパイルオプション  
`-mcpu=rx230 -misa=v2 -mlittle-endian-data -ffunction-sections -fdata-sections`  
リンクオプション  
`-mcpu=rx230 -misa=v2 -mlittle-endian-data -ffunction-sections -fdata-sections -nostartfiles -nostdlib`

## 2.3 インクルードパス
μT-Kernel3.0のソースディレクトリの以下のディレクトリを指定する必要がある。

|ディレクトリパス|内容|
|-|-|
|config|コンフィギュレーションファイル|
|include|共通ヘッダファイル|
|kernel\knlinc|カーネル内共通ヘッダファイル|

kermel\knlincはOS内部でのみ使用するヘッダファイルである。ユーザプログラムについては、config とincludeのみを使用する。

## 2.4 標準ライブラリ
本ソフトは基本的にはコンパイラの標準ライブラリを使用しない。よって、リンク時のオプションで-nostdlibを指定している。  
ただし、ユーザのアプリケーションや使用するライブラリなどによっては、標準ライブラリが必要となる場合がある。

# 3. 開発環境と構築手順
本ソフトをビルドするための開発環境とそれを用いた構築手順を説明する。

本ソフトは極力、特定の開発環境に依存しないように作られている。ここでは例として、WindowsのPCにおいて、ルネサス エレクトロニクスの統合開発環境e2 studioを使用する場合と、自動ビルドツールMakeを使用する場合を説明する。  
なお、ここに示す開発環境や構築手順はあくまで例であり、ユーザそれぞれの環境などによって差異がある場合がある。

## 3.1 e2 studioを使用した構築手順
### 3.1.1 e2 studioの準備
(1) e2 studioのインストール  
e2studioは、オープンソースの"Eclipse"をベースとした、ルネサス製マイコン用の統合開発環境である。

本ソフトの動作検証にはe2 studioの以下のバージョンを使用した。

`e2 studio 2023-07 (23.7.0)`

e2 studioは以下のe2 studioのホームページからインストーラが入手可能である。なお、ダウンロードにはユーザ登録が必要である。

https://www.renesas.com/jp/ja/products/software-tools/tools/ide/e2studio.html

インストーラによるe2 studioのインストールの際には、対象デバイスとしてRXマイコンを選択する。

e2 studioのインストールや操作については、上記のホームページを参照のこと。

(2) ワークスペースの作成  
e2 studioの初回起動時、指示に従いワークスペースを作成する。ワークスペースは、e2 studioの各種設定などが保存される可能的な作業場である。

### 3.1.2 プロジェクトの作成
E2 studioにて以下の手順で本ソフトのプロジェクトを作成する。

(1) メニュー「新規」→「C/C++ プロジェクト」を選択する。  
開いた新規C/C++プロジェクトのテンプレート画面で「C Managed Build」を選択する。
次の「Cプロジェクト」画面で以下を設定する。
 - プロジェクト名：任意
 - ロケーション：任意
 - プロジェクトの種類：「実行可能」→「空のプロジェクト」
 - ツールチェーン：「GCC for Renesas RX」

(2) メニュー「ファイル」→「インポート…」を選択する。  
開いた選択画面で「一般」→「ファイルシステム」を選択し、ファイルシステム画面で本ソフトのソースコードのディレクトリを入力する。  
なお、(1)でプロジェクトのロケーションに、既にソースコードのディレクトリが存在するディレクトリを指定した場合は、インポートは不要である。

(3) メニュー「プロジェクト」→「プロパティ」を選択する。  
以降、プロパティのダイアログにて各項目を設定していく。なお、本書の設定は一例であり、必要に応じて変更すること。  

(4) ダイアログの項目「C/C++ビルド」→「設定」を選択し、「ツール設定」開いて以下の設定を行う。  

|||
|-|-|
|CPU|「CPU Type」に「RX230」を選択|
|Optinmization|「Optimaization Level」は任意<br>オプションは「-ffunction-sections」と「-fdata-sections」のみ選択|
|Compiler|「Source」<br>　　「Language standard」に「GNU ISO C11 (-std=gnu11)」を選択<br>「Includes」<br>　　「Include file directories」にインクルードパスの追加<br>　　μT-Kernel3.0のインクルードパスを設定する。「2.3インクルードパス」を参照<br>　　「Macro Defines」にターゲット名を定義。<br>　　　　\_IOTE_RX231\_|
 |Assembler|「Source」<br>　　「User defined option」にターゲット名を定義<br>　　　　-D_IOTE_RX231_<br>「Includes」<br>　　「Include file directories」にインクルードパスの追加。「2.3インクルードパス」を参照|
|Linker|「Source」<br>　　「Entry point」に以下を設定<br>　　　　-WI,-e_Reset_Handler<br>　　     「Linker script」にスクリプト・ファイルのパスを設定する。<br>　　　　　tkermnel_3\etc\linker\iote_rx231\tkernel_map.ld|


(5) ダイアログの項目「C/C++ビルド」→「設定」を選択し、「Toolchain」タブを開いて以下の設定を行う。  

|||
|-|-|
|ツールチェーン|「GCC for Renesas RX」を選択する。<br>「バージョン」：任意|

全ての設定が完了したら「適用して閉じる」ボタンを押下する。

### 3.1.3 プロジェクトのビルド
メニュー「プロジェクト」→「プロジェクトのビルド」を選択すると、本ソフトのソースコードがコンパイル、リンクされ、実行コードのELFファイルが生成される。

## 3.2 Makeを使用したビルド方法
### 3.2.1 開発ソフトの準備
Cコンパイラなど共通の開発ツールをインストールする。

(1) Cコンパイラのインストール
GCCコンパイラ一式を以下から入手し、Webページの指示に従いインストールする。

Open Source Tools for RENESAS
https://llvm-gcc-renesas.com/ja/rx-download-toolchains/

本稿作成時に検証したバージョンは以下の通り。
`rx-elf-gcc.exe (GCC_Build_20230525) 8.3.0.202305-GNURX 20190222`

(2) 開発ツールのインストール
開発ツール一式（makeなど）を以下から入手し、Webページの指示に従いインストールする。

xPack Windows Build Tools
https://xpack.github.io/windows-build-tools/

本稿作成時に検証したバージョンは以下の通り。
`xPack Windows Build Tools v4.4.1-2`

### 3.2.2 ビルド環境の準備
(1) 実行パスの設定
Windowsのコマンドシェル（PowerShellまたはコマンドプロンプト）から、GCCおよびMakeが実行可能となるように、環境変数pathにGCCを展開したディレクトリ内の\binディレクトリのパスおよび、xPack Windows Build Toolsを展開したディレクトリ内の\binディレクトリのパスを追加設定する。  
コマンドシェルからGCC（rx-elf-gcc）およびmakeコマンドが実行可能であることを確認する。

(2) makefileの設定
本ソフトのソースコード中のMake用ビルドディレクトリ(build_make)にmakefileが格納されている。  
ディレクトリ(build_make)の内容を以下に示す。  

|名称|説明|
|-|-|
|makefile|μT-Kernel 3.0のビルド規則（ルート）|
|iote_m367.mk|M367 IoT-Engine用のビルド規則(※)|
|iote_rx231.mk|RX231 IoT-Engine用のビルド規則|
|iote_stm32l4.mk|STM32L4 IoT-Engine用のビルド規則(※)|
/mtkernel_3|Make作業用ディレクトリ|

※ 本書の説明では使用しない。

makefileファイルの先頭の以下の定義を変更する。  

|定義名|初期値|説明|
|-|-|-|
|EXE_FILE|mtkernel_3|ビルドする実行ファイル名|
|TARGET|\_IOTE_M367\_|対象とするハードウェア<br>M367 IoT-Engineの場合は「\_IOTE_RX231\_」に変更する|

また、iote_rx231.mkの先頭の以下の定義を必要に応じて変更する。  

|定義名|初期値|説明|
|-|-|-|
|GCC|rx-elf-gcc|Cコンパイラのコマンド名|
|AS|rx-elf-gcc|アセンブラのコマンド名|
|LINK|rx-elf-gcc|リンカのコマンド名|
|CFLAGS|省略(※)|Cコンパイラのオプション|
|ASFLAGS|省略(※)|アセンブラのオプション|
|LFLAGS|省略(※)|リンカのオプション|
|LINKFILE|省略(※)|リンク定義ファイル|

※ iote_rx231.mkファイルの記述を参照

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
アプリケーションには、usermain関数を定義する。OSは起動後に初期タスクからusermain関数を実行する。詳細はμT-Kernel3.0共通実装仕様書「5.2.3 ユーザ定義メイン関数usermain」を参照のこと。

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
ビルドしたプログラムを実機上で実行する方法を、実機にRX231 IoT-Engine Arduino Evaluation Kitを使用し、エミュレータにはルネサス エレクトロニクス製のE1を使用した場合を例に説明する。  
前章で説明したE2 studioの開発環境から実機に実行コードを転送し、実行、デバッグを行う。  

## 5.1 E2 studioによるプログラムの実行
(1) E2 studioのメニューからメニュー「実行」→「デバッグの構成」を選択し、開いたダイアログから項目「Renesas GDB Hardware Debugging」を選択する。

(2) 「新規構成」ボタンを押し、「Renesas GDB Hardware Debugging」に構成を追加する。

(3) 追加した構成を選択し、「構成の作成、管理、実行」画面にて以下の設定を行う。  

|||
|-|-|
|「メイン」タブ|名前：（任意）を入力<br>C/C++アプリケーション：ビルドしたELFファイル|
|「Debugger」タブ|「GDB Settings」<br>　　Debug Hardware：「E1 (RX)」を選択<br>　　Target Deveice：「R5F52318」を選択<br>「Connection Setting」<br>　　「クロック」→「EXTAL周波数」8.0000を入力<br>　　「電源」→「エミュレータから電源を供給する」いいえを選択|
|「Startup」タブ|ブレークポイント設定先：「user_main」を入力|

(4) デバッグ開始
「デバッグ」ボタンを押すとプログラムが実機に転送され、ROMに書き込まれたのち、実行される。  
プログラムは実行すると、OS起動後にユーザのアプリケーションプログラムを実行し、user_main関数にてブレークする。

# 更新履歴

|版数|日付|内　容|
|-|-|-|
|2.00.00|2023.12.01|開発環境のバージョンの更新<br>ドキュメントフォーマットの変更<br>全体の見直しおよび変更|
|1.00.04|2022.06.30|開発環境のバージョンの更新|
|1.00.03|2021.05.17|開発環境のバージョンの更新<br>全体の見直しおよび変更|
|1.00.02|2020.10.21|4.1 E2 studioによるプログラムの実行<br>　サンプルのファイル名変更 (旧)usermain.c  (新)app_main.c|
|1.00.01|2020.05.29|3. 構築手順<br>　Eclipseに加えてMakeによる構築手順を記載<br>開発ツールのバージョン更新<br>その他（文章の直し、誤字修正など）|
|1.00.00|2020.03.13| 初版|