# PocketStation C Cross Compiler
ポケットステーション用gcc構築


## インストール

### 準備
* このビルドにはbinutils、gccのパッケージが必要だが、含まれていない
* 以下のパッケージをダウンロードして`package/`ディレクトリに置くこと (URLはpackage/pkglist)
    * binutils-2.27.tar.gz
    * gcc-3.4.6.tar.gz
* package内で以下のコマンドを実行するとダウンロードできる
    * `wget -i pkglist`
* ソースコードを既に展開している場合、以下のコマンドで`package/`がなくてもエラーにならない
    * `make install PAC_BIN= PAC_GCC=`

### ビルド、インストール
* 以下のコマンドでビルド、インストールを行う (make のみは何もしない)
    * `make install`

* gccをビルドするためにはターゲット用のbinutilsを使用する必要がある
    * そのため先にbinutilsの`make`, `make install`を実行する
* その後、インストールしたbinutilsを使用してgccの`make`, `make install`を実行する

* `DESTDIR`を指定する場合でも先にbinutilsを適当な場所にインストールしてから
* `DESTDIR`を指定したビルドを実行する必要がある
* 例:
    * `make build_bin install_bin`
    * `make build_bin build_gcc install_bin install_gcc INSTDIR=/usr/local DESTDIR=$HOME/ppp PKS_BINDIR=$HOME/pks_toolchain/bin`

* cygwin64ビット版でビルドする場合、binutilsの一部とgccのconfig.guessがcygwin64を認識できないので`--build` `--host`を指定する必要がある
* 例:
    * `make install CONF_FLAG_BIN=--build=i686-pc-cygwin CONF_FLAG_GCC=--host=i686-pc-cygwin`

### その他
* `libiberty`は使用しない
    * libibertyは GNUプログラムが利用するサブルーチンを提供する (getopt obstack等)
    * 参考 <http://lfsbookja.osdn.jp/8.0-ja/chapter06/gcc.html>


## 使用
* `etc/ldscripts/arm_pocketstation.x`がポケットステーション用リンカスクリプト
    * scheaderセクションを用意している
    * scheaderセクションにヘッダを記述するとtextセクションの開始位置がフレームサイズでアラインメントされる
    * scheader無しでtextセクションにヘッダ全てを書いてもいい
