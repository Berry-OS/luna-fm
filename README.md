# Luna Files

`luna-ui.h` を使用した、PCManFM風の操作性とmacOS風の外観を持つLinux向けファイルマネージャです。

## 主な機能

- サイドバー: ホーム、デスクトップ、書類、ダウンロード、画像、音楽、動画、ごみ箱、ルート
- 戻る／進む／上へ／再読込、場所バー、フォルダー内検索
- アイコン表示／リスト表示、隠しファイル切替、名前・日時・サイズ順の切替
- 単一・Ctrl複数・Shift範囲選択、ダブルクリックで開く、右クリックメニュー
- 新規フォルダー、名前変更、情報表示、ごみ箱への移動
- Ctrl+C / Ctrl+X / Ctrl+V のファイルクリップボード
- Wayland `wl-copy` / `wl-paste`、X11 `xclip` の `x-special/gnome-copied-files` に任意対応
- `wl-copy` / `xclip` がない場合も、アプリ内クリップボードとGLFWテキストクリップボードで動作

## 必要ファイル

このフォルダーへ、Luna UIが元から使用している次のヘッダーを置いてください。

- `stb_truetype.h`
- `stb_image.h`
- `stb_image_write.h`
- `cssparser.h`

`luna-ui.h` は同梱済みで、ホスト側ショートカットのため `luna_focused_element()` と `luna_focus_element()` を追加しています。

## ビルド

Debian/Ubuntu系の例:

```sh
sudo apt install build-essential pkg-config libglfw3-dev libgl1-mesa-dev
make
```

起動:

```sh
./luna-fm
./luna-fm /開きたい/フォルダー
```

Waylandで他のファイルマネージャとのコピー＆貼り付けを強化する場合:

```sh
sudo apt install wl-clipboard
```

X11の場合:

```sh
sudo apt install xclip
```

## 操作

- `Enter`: 開く
- `Backspace`: 戻る
- `F2`: 名前変更
- `Delete`: ごみ箱へ移動
- `Ctrl+A`: すべて選択
- `Ctrl+C`: コピー
- `Ctrl+X`: 切り取り
- `Ctrl+V`: 貼り付け
- `Ctrl+L`: 場所バー
- `Esc`: メニュー／ダイアログを閉じる

## 実装上の方針

Luna UIのDOMをフォルダー移動ごとに再解析せず、384個の表示スロットを最初に作成し、`luna_set_text()` とクラス更新だけで差し替えています。これにより、一覧更新・検索・表示切替で不要なHTML再解析を避けています。
