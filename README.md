# ATOM Matrix Web Pixel Studio

Paint all 25 RGB LEDs individually from a web browser on an M5Stack ATOM Matrix.

WebブラウザーからM5Stack ATOM Matrixの25個のRGB LEDを1つずつ塗るサンプルです。

[日本語](#日本語) | [English](#english)

## Screenshot / スクリーンショット

![ATOM Matrix Web Pixel Studio interface](docs/images/web-interface.png)

---

## 日本語

### 概要

ATOM Matrix上でWebサーバーを動かし、スマートフォンやPCから5×5 RGB LEDの色を個別に編集します。
Web画面で色を選び、25個のLEDをクリックまたはドラッグして塗ってから「ATOM Matrixへ反映」を押すと、パターンが実機へ反映されます。

反映した25色はRAMだけに保持され、不揮発メモリ（NVS）へは保存されません。リセットまたは電源を入れ直すと、毎回ソースコード内の初期パターンへ戻ります。
通常の2.4 GHz Wi-Fiへ接続できない場合は、ATOM Matrix自身がアクセスポイントになり、Wi-Fiルーターなしでも操作できます。

### 機能

- 5×5のRGB LEDを1個ずつカラー指定
- カラーピッカー、`#RRGGBB`入力、7色のプリセットに対応
- クリック操作とドラッグ操作に対応
- 「すべて消灯」と「本体から再読込」を用意
- 25色をまとめて実機へ反映し、その起動中だけRAMに保持
- リセットまたは電源再投入時に初期パターンへ自動復帰
- 通常のWi-Fi接続と、接続失敗時のアクセスポイントモードに対応
- レスポンシブ対応のWeb画面をファームウェアへ内蔵
- 外部Webサーバーや画像アセットは不要

### 安全上の注意

> [!CAUTION]
> [M5Stack公式ドキュメント](https://docs.m5stack.com/ja/core/ATOM%20Matrix)では、FastLED使用時の推奨輝度は`20`と案内されており、高輝度や全点灯は発熱によってLEDやアクリルスクリーン、筐体を損傷するおそれがあるとされています。
> このサンプルは安全のため、Adafruit NeoPixelの輝度を`20 / 255`へ固定しています。`LED_BRIGHTNESS`を安易に上げず、長時間使用中に本体が熱くなった場合は消灯してUSBケーブルを外してください。

### 必要なもの

- [M5Stack ATOM Matrix](https://docs.m5stack.com/ja/core/ATOM%20Matrix)
- データ通信対応USB Type-Cケーブル
- Windows、macOS、またはLinuxを搭載したPC
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE for VS Code](https://docs.platformio.org/en/latest/integration/ide/vscode.html)
- 2.4 GHz Wi-Fi（家庭や社内のWi-Fiへ接続する場合）

> [!IMPORTANT]
> ATOM Matrixは5 GHz Wi-Fiへ接続できません。`XXXX-5G`などではなく、2.4 GHz側のSSIDを設定してください。

### セットアップ

#### 1. プロジェクトを開く

このリポジトリをGitでクローンするか、GitHubの **Code** → **Download ZIP** からダウンロードして展開します。
VS Codeを起動し、**ファイル** → **フォルダーを開く** から、`platformio.ini`があるフォルダーを開きます。

PlatformIOを初めて使用する場合は、VS Codeの拡張機能から `PlatformIO IDE` をインストールし、VS Codeを再起動します。

#### 2. Wi-Fiを設定する

`include/wifi_config.example.h`をコピーし、コピーしたファイルの名前を`wifi_config.h`へ変更します。

```text
include/
├── wifi_config.example.h
└── wifi_config.h          ← 作成するファイル
```

`include/wifi_config.h`を開き、接続する2.4 GHz Wi-FiのSSIDとパスワードを入力します。

```cpp
#pragma once

#define WIFI_SSID "YOUR_2_4_GHZ_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

`include/wifi_config.h`は`.gitignore`へ登録されているため、通常はGitHubへアップロードされません。公開前には、実際のSSIDやパスワードが含まれていないことを必ず確認してください。

Wi-Fiを設定せず、後述のアクセスポイントモードだけで使用することもできます。

#### 3. ATOM Matrixへ書き込む

1. ATOM Matrixをデータ通信対応USB Type-CケーブルでPCへ接続します。
2. VS Code下部にあるPlatformIOのチェックマーク（Build）を押します。
3. 矢印（Upload）を押してファームウェアを書き込みます。
4. プラグアイコン（Serial Monitor）を押し、115200 bpsで起動メッセージを確認します。

PlatformIO Coreを使用する場合は、プロジェクトのフォルダーで次のコマンドを実行できます。

```shell
# ビルド
pio run

# ATOM Matrixへ書き込み
pio run --target upload

# シリアルモニター
pio device monitor --baud 115200
```

書き込みポートはPlatformIOが自動検出します。複数のシリアル機器が接続されている場合は、PlatformIOから対象ポートを選択してください。

### Web画面を開く

#### 通常のWi-Fiへ接続できた場合

シリアルモニターへ次のように表示されます。

```text
Wi-Fi connected.
URL: http://atom-matrix.local/
IP : http://192.168.1.23/
Web server started.
```

ATOM Matrixと同じネットワークへ接続したスマートフォンまたはPCから、次のいずれかを開きます。

- `http://atom-matrix.local/`
- シリアルモニターへ表示されたIPアドレス（例：`http://192.168.1.23/`）

端末やネットワークによっては`.local`を利用できません。その場合はIPアドレスを使用してください。

#### アクセスポイントモードの場合

Wi-Fi設定がない場合、または15秒以内に指定したWi-Fiへ接続できなかった場合、ATOM Matrix自身がアクセスポイントになります。

```text
Access point mode started.
SSID: AtomMatrix-LED
PASS: atommatrix
URL : http://192.168.4.1/
```

スマートフォンまたはPCを次のWi-Fiへ接続してから、ブラウザーでURLを開きます。

- Wi-Fi名：`AtomMatrix-LED`
- パスワード：`atommatrix`
- URL：`http://192.168.4.1/`

「インターネットへ接続されていません」と表示されても、この操作では問題ありません。

### LEDパターンを作る

1. Web画面のカラーピッカー、カラーコード、または丸いプリセットから色を選びます。
2. 5×5プレビューのLEDをクリックします。複数を続けて塗る場合はドラッグします。
3. 必要に応じて「すべて消灯」や「本体から再読込」を使用します。
4. 「ATOM Matrixへ反映」を押します。
5. 25色がLEDへ反映され、その起動中だけRAMに保持されます。

LED番号は、ATOM Matrixを正面から見て左上の`01`から右下の`25`まで、行ごとに並びます。

```text
01  02  03  04  05
06  07  08  09  10
11  12  13  14  15
16  17  18  19  20
21  22  23  24  25
```

編集内容は「ATOM Matrixへ反映」を押すまでWeb画面内のプレビューだけに保持されます。
「本体から再読込」を押すと、未送信の編集を破棄して、現在本体のRAMに保持されている25色を読み直します。
リセットまたは電源再投入後は、`DEFAULT_COLORS`で定義した初期パターンへ戻ります。

### プロジェクト構成

```text
.
├── docs/
│   └── images/
│       └── web-interface.png    Web画面のスクリーンショット
├── include/
│   ├── wifi_config.example.h    Wi-Fi設定テンプレート
│   └── wifi_config.h            ローカルWi-Fi設定（Git管理対象外）
├── src/
│   ├── main.cpp                 Wi-Fi、Web API、LED制御
│   └── web_page.h               Web画面のHTML、CSS、JavaScript
├── .gitignore
├── LICENSE
├── platformio.ini               PlatformIOビルド設定
└── README.md
```

### 主な設定

`src/main.cpp`の先頭付近で変更できます。

| 設定 | 初期値 | 説明 |
| --- | --- | --- |
| `LED_PIN` | `27` | ATOM Matrix内蔵RGB LEDのGPIO |
| `LED_COUNT` | `25` | 5×5マトリクスのLED数 |
| `LED_BRIGHTNESS` | `20` | 安全のため固定した全体輝度（0～255） |
| `WIFI_TIMEOUT_MS` | `15000` | Wi-Fi接続を待つ時間（ミリ秒） |
| `HOST_NAME` | `atom-matrix` | `.local`アクセスに使用するホスト名 |
| `AP_SSID` | `AtomMatrix-LED` | アクセスポイントモードのSSID |
| `AP_PASSWORD` | `atommatrix` | アクセスポイントモードのパスワード |

> [!CAUTION]
> 公共の場所や共有環境で使用する場合は、`AP_PASSWORD`を別の8文字以上のパスワードへ変更してください。

### Web API

| メソッド | パス | 説明 |
| --- | --- | --- |
| `GET` | `/api/leds` | 25色と固定輝度をJSONで取得 |
| `POST` | `/api/leds` | 25色をLEDへ反映し、RAMだけに保持 |

POSTデータは`application/x-www-form-urlencoded`形式です。`colors`へ`#RRGGBB`形式の25色をカンマ区切りで指定します。

```text
colors=#FF0000,#000000,...,#0000FF
```

### トラブルシューティング

#### `AtomMatrix-LED`が表示され、指定したWi-Fiへ接続されない

- SSIDが2.4 GHz側であることを確認します。
- SSIDとパスワードの大文字・小文字を確認します。
- `include/wifi_config.h`を保存した後、もう一度Uploadします。
- ATOM MatrixをWi-Fiルーターへ近づけます。

#### `http://atom-matrix.local/`を開けない

- ATOM Matrixと操作端末が同じネットワークに接続されているか確認します。
- シリアルモニターに表示されたIPアドレスをブラウザーで開きます。
- ゲストWi-Fiでは端末同士の通信が禁止されている場合があります。

#### 書き込みポートが表示されない

- 充電専用ではなく、データ通信対応USBケーブルを使用します。
- USBケーブルを挿し直すか、別のUSBポートを試します。
- 書き込み前に、ポートを使用している別のシリアルモニターを閉じます。
- Windowsで未認識の場合は、[ATOM Matrix公式ページ](https://docs.m5stack.com/ja/core/ATOM%20Matrix)の案内に従ってFTDI VCPドライバーを確認します。

---

## English

### Overview

This project runs a web server on an M5Stack ATOM Matrix and lets a smartphone or computer edit each color in its 5×5 RGB LED matrix.
Choose a color in the web interface, click or drag across the 25-pixel preview, and press **Apply to ATOM Matrix** to update the physical LEDs.

Applied colors are kept in RAM only and are never saved to non-volatile storage (NVS). Every reset or power cycle restores the initial pattern defined in the source code.
If the device cannot connect to a regular 2.4 GHz Wi-Fi network, it starts its own access point and remains usable without a Wi-Fi router.

### Features

- Set a separate RGB color for every pixel in the 5×5 matrix
- Choose colors with a color picker, `#RRGGBB` input, or seven presets
- Paint pixels by clicking or dragging
- Turn every pixel off or reload the current in-memory pattern
- Apply all 25 colors together and keep them in RAM for the current boot
- Restore the initial pattern automatically after a reset or power cycle
- Support regular Wi-Fi and fallback access point modes
- Provide a responsive web interface embedded in the firmware
- Require no external web server or image assets

### Safety

> [!CAUTION]
> The [official M5Stack documentation](https://docs.m5stack.com/en/core/ATOM%20Matrix) recommends a brightness of `20` when using FastLED and warns that excessive brightness or lighting every pixel can generate enough heat to damage the LEDs, acrylic screen, or enclosure.
> For safety, this sample fixes the Adafruit NeoPixel brightness at `20 / 255`. Do not casually increase `LED_BRIGHTNESS`. If the device becomes hot during extended use, turn the pixels off and disconnect the USB cable.

### Requirements

- [M5Stack ATOM Matrix](https://docs.m5stack.com/en/core/ATOM%20Matrix)
- A data-capable USB Type-C cable
- A Windows, macOS, or Linux computer
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE for VS Code](https://docs.platformio.org/en/latest/integration/ide/vscode.html)
- A 2.4 GHz Wi-Fi network for station mode

> [!IMPORTANT]
> ATOM Matrix cannot connect to a 5 GHz Wi-Fi network. Select the 2.4 GHz SSID instead of an SSID such as `XXXX-5G`.

### Setup

#### 1. Open the project

Clone this repository with Git, or download and extract it using **Code** → **Download ZIP** on GitHub.
Start VS Code and open the folder containing `platformio.ini` using **File** → **Open Folder**.

If this is your first PlatformIO project, install `PlatformIO IDE` from the VS Code Extensions view and restart VS Code.

#### 2. Configure Wi-Fi

Copy `include/wifi_config.example.h` and rename the copy to `wifi_config.h`.

```text
include/
├── wifi_config.example.h
└── wifi_config.h          ← create this file
```

Open `include/wifi_config.h` and enter the SSID and password of your 2.4 GHz Wi-Fi network.

```cpp
#pragma once

#define WIFI_SSID "YOUR_2_4_GHZ_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

`include/wifi_config.h` is listed in `.gitignore`, so it is normally excluded from Git commits. Always confirm that your actual SSID and password are not included before publishing the repository.

You can also leave Wi-Fi unconfigured and use access point mode only.

#### 3. Upload to the ATOM Matrix

1. Connect the ATOM Matrix to your computer with a data-capable USB Type-C cable.
2. Click the PlatformIO checkmark (**Build**) in the VS Code status bar.
3. Click the arrow (**Upload**) to upload the firmware.
4. Click the plug icon (**Serial Monitor**) and check the startup messages at 115200 bps.

If you use PlatformIO Core, run the following commands from the project directory:

```shell
# Build
pio run

# Upload to the ATOM Matrix
pio run --target upload

# Open the serial monitor
pio device monitor --baud 115200
```

PlatformIO detects the upload port automatically. Select the correct port in PlatformIO if multiple serial devices are connected.

### Open the web interface

#### When connected to your regular Wi-Fi network

The serial monitor displays messages similar to the following:

```text
Wi-Fi connected.
URL: http://atom-matrix.local/
IP : http://192.168.1.23/
Web server started.
```

Connect your smartphone or computer to the same network as the ATOM Matrix and open one of the following URLs:

- `http://atom-matrix.local/`
- The IP address shown in the serial monitor, such as `http://192.168.1.23/`

Some devices and networks do not support `.local` hostnames. Use the IP address in that case.

#### When running in access point mode

If Wi-Fi is not configured or the ATOM Matrix cannot connect within 15 seconds, it starts its own access point.

```text
Access point mode started.
SSID: AtomMatrix-LED
PASS: atommatrix
URL : http://192.168.4.1/
```

Connect your smartphone or computer to the following Wi-Fi network, then open the URL in a browser:

- Wi-Fi name: `AtomMatrix-LED`
- Password: `atommatrix`
- URL: `http://192.168.4.1/`

You can ignore a warning that this Wi-Fi network has no internet connection.

### Create an LED pattern

1. Choose a color using the color picker, color code field, or round presets.
2. Click a pixel in the 5×5 preview. Drag across the grid to paint several pixels.
3. Use **All off** or **Reload from device** when needed.
4. Press **Apply to ATOM Matrix**.
5. The firmware updates all 25 LEDs and keeps their colors in RAM for the current boot only.

Pixel numbers run row by row from `01` at the top left to `25` at the bottom right when viewing the front of the ATOM Matrix.

```text
01  02  03  04  05
06  07  08  09  10
11  12  13  14  15
16  17  18  19  20
21  22  23  24  25
```

Edits remain only in the browser preview until you press **Apply to ATOM Matrix**.
Pressing **Reload from device** discards unsent edits and reloads the 25 colors currently held in device RAM.
A reset or power cycle restores the initial pattern defined by `DEFAULT_COLORS`.

### Project structure

```text
.
├── docs/
│   └── images/
│       └── web-interface.png    Web interface screenshot
├── include/
│   ├── wifi_config.example.h    Wi-Fi configuration template
│   └── wifi_config.h            Local Wi-Fi settings, ignored by Git
├── src/
│   ├── main.cpp                 Wi-Fi, Web API, and LED control
│   └── web_page.h               Embedded HTML, CSS, and JavaScript
├── .gitignore
├── LICENSE
├── platformio.ini               PlatformIO build configuration
└── README.md
```

### Main configuration values

The following values can be changed near the top of `src/main.cpp`.

| Setting | Default | Description |
| --- | --- | --- |
| `LED_PIN` | `27` | GPIO used by the built-in RGB matrix |
| `LED_COUNT` | `25` | Number of pixels in the 5×5 matrix |
| `LED_BRIGHTNESS` | `20` | Safety-limited global brightness from 0 to 255 |
| `WIFI_TIMEOUT_MS` | `15000` | Wi-Fi connection timeout in milliseconds |
| `HOST_NAME` | `atom-matrix` | Hostname used for `.local` access |
| `AP_SSID` | `AtomMatrix-LED` | SSID used in access point mode |
| `AP_PASSWORD` | `atommatrix` | Password used in access point mode |

> [!CAUTION]
> Change `AP_PASSWORD` to a different password of at least eight characters before using this project in a public or shared environment.

### Web API

| Method | Path | Description |
| --- | --- | --- |
| `GET` | `/api/leds` | Get the 25 colors and fixed brightness as JSON |
| `POST` | `/api/leds` | Apply the 25 colors and keep them in RAM only |

POST data uses the `application/x-www-form-urlencoded` format. Pass 25 comma-separated `#RRGGBB` values in `colors`.

```text
colors=#FF0000,#000000,...,#0000FF
```

### Troubleshooting

#### `AtomMatrix-LED` appears instead of connecting to the configured Wi-Fi

- Confirm that the selected SSID uses the 2.4 GHz band.
- Check the capitalization of the SSID and password.
- Save `include/wifi_config.h` and upload the firmware again.
- Move the ATOM Matrix closer to the Wi-Fi router.

#### `http://atom-matrix.local/` does not open

- Confirm that the ATOM Matrix and browser device are on the same network.
- Open the IP address displayed in the serial monitor.
- Guest Wi-Fi networks may prevent devices from communicating with one another.

#### No upload port is available

- Use a data-capable USB cable rather than a charge-only cable.
- Disconnect and reconnect the cable, or try another USB port.
- Close any other serial monitor using the port before uploading.
- On Windows, follow the FTDI VCP driver guidance on the [official ATOM Matrix page](https://docs.m5stack.com/en/core/ATOM%20Matrix) if the device is not recognized.

## License

This project is released under the [MIT License](LICENSE).
