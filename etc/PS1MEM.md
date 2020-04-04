# PS1のメモリーカードの情報


## 各種フォーマット
### MCB形式 (mcb mcx) ブロック単位
* ヘッダの後にブロックが複数個 (54b + 8192b * ブロック数)
* ヘッダ 54byte
    * データ名(20byte + 0x00) 21byte
    * セーブデータ名(32byte + 0x00) 33byte
* ポケットステーションでの実行用のものをmcx、それ以外をmcbとしている？

### MC形式 (mc mcd mci mcr ps) メモリ全体
* メモリーカードのデータそのもの (128kb固定)
* メモリーカードの先頭にasciiで`MC`の2バイトが付くので、代表してMC形式とする

### MEM形式 (mem) メモリ全体
* 詳細不明
* メモリーカード全体のデータになにかのヘッダが付いている


## Stirlingの構造体編集用設定
MC形式
```
/* PS1メモリーカードの構造 */

/* フレーム 基本 */
struct PS1MC_FRAME
{
	BYTE data[128];
};

/* 管理ブロック フレーム0 */
struct PS1MC_MANBLOCK_IDFRAME
{
	BYTE magic_MC[2];
	BYTE data[125];
	BYTE xor;          /* check sum */
};

/* 管理ブロック フレーム1～15 ブロック情報フレーム */
struct PS1MC_MANBLOCK_INFOFRAME
{
	BYTE flag;
	BYTE unused1[3];
	DWORD used_size;   /* little endian */
	BYTE link[2];
	BYTE name[20];
	BYTE unused2[97];
	BYTE xor;          /* check sum */
};

/* 管理ブロック */
struct PS1MC_MANBLOCK 
{
	PS1MC_MANBLOCK_IDFRAME idframe;
	PS1MC_MANBLOCK_INFOFRAME infoframe[15];
	PS1MC_FRAME unused[47];
	PS1MC_FRAME rw_check;
};

/* データブロック */
struct PS1MC_DATABLOCK
{
	PS1MC_FRAME frame[64];
};

/* メモリ全体 */
struct PS1MC
{
	PS1MC_MANBLOCK manblock;
	PS1MC_DATABLOCK datablock[15];
};
```
