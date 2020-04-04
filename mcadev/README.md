# mcadev
メモリーカードアダプタ用デバイスドライバ (キャラクタデバイス)

* Ubuntu 16.04 でビルド、動作確認
  * ビルドには追加パッケージ不要
  * /lib/modules/ 以下に必要なファイルが最初からあるため

* mcaps1m
  * ps1メモリーカード用
* mcac
  * メモリーカードの種類確認 read only


## ビルド
* `make`


## 使い方
メモリーカードデータ1つ分(128kb)単位での読み書き

* mcaps1m
  * 読み込み
    * `cat /dev/mcaps1m > read_data.ps`
  * 書き込み
    * `cat write_data.ps > /dev/mcaps1m`
* mcac MCAのメモリカード確認
  * `cat /dev/mcac > mc_type`


## 処理内容
* バッファを持たない
* read()内での読み込みは常に1回
* write()内での書き込みは常に1回 (ただし、書き込み前に読み込みを1回行う可能性がある)
* open でデバイスの先頭になる
* read
    * 基本的にセクタ単位(128byte)での読み込み
    * ただしセクタをまたぐサイズの場合は128byte未満となる
    * 全て読んだらEOFとする
* write
    * 基本的にセクタ単位(128byte)での書き込み
    * ただしセクタをまたぐサイズの場合は以下の条件で処理を行う
      * 書き込みはセクタ境界までで、128byte未満となる
      * 対象のセクタを最初に読み込み、該当部分を書き換えた上で書き込みを行う
    * デバイスサイズ超過する場合、可能な限り書き込み、ENOSPC(デバイスに十分な空きがない)とする
      * 書き込み位置がMCサイズを超えている場合が当てはまる
