#ifndef UARTPRINT_H
#define UARTPRINT_H

//extern int UART1_copied_SSR;	//送信直後のｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀの値
//extern int UART1_TXF;		//送信作業中だと１
//extern int UART1_TIMEOUT;	//待ち処理でタイムアウト発生すると１
//extern char UART1_TXbuf[];	//送信処理で参照されるﾊﾞｯﾌｧ

void UART1_TX_init(void);			//SCI1を使ったUART送信の初期設定をします。
void UART1_transmit(const char* InputAddress);	//この関数を書くと送信実行します。引数は文字配列の変数名など。
void UART1_TXI_interrupt(void);	//送信エンプティ割込み ここでTDRﾚｼﾞｽﾀに値を1度だけ書き込む。
void UART1_TEI_interrupt(void);	//送信完了割込み 送信終了の処理 フラグ解除と状態ﾚｼﾞｽﾀコピーも。
void UART1_vtransmit(const char *format, ...);
//void UART1_address_set(char *target);

//void UART1_renew(char *format, ...);
//void UART1_execute(void);

int getssr(void);		//シリアルステータスﾚｼﾞｽﾀの値を取得する関数。
int getstate(void);		//送信処理フラグを取得する関数。送信処理の途中だと１が返ってくるはず。
int gettimeout(void);	//タイムアウト判定結果を取得する関数。タイムアウトしてた場合は１が返ってくるはず。

#endif
