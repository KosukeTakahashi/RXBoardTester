/******************* UART 送信フォーマット設定 *******************************************/
#define UART1_TXBUF_SIZE 64				//ライブラリ内に準備しておく送信バッファサイズ[B]をここで指定 超えると動作不安定の恐れあり。
#define UART1_PARITY 0					//パリティビット 　 あり[1] or なし[0]
#define UART1_PARITYMODE 0				//パリティモード　奇数ﾊﾟﾘﾃｨ[1] or 偶数ﾊﾟﾘﾃｨ[0] ﾊﾟﾘﾃｨあり時のみ有効
#define	UART1_STOPBIT 1					//ストップビット  　[1]ビット or [2]ビット
#define UART1_N_VALUE 19				//ﾎﾞｰﾚｰﾄﾚｼﾞｽﾀ  N の値 (RX62Gではp.918および設定例参照) => PCLK=25MHz で 38400bps
#define UART1_n_VALUE 0					//クロック選択ビット n の値 (RX62Gではp.918および設定例参照)
#define TIMEOUT_COUNTER_DEFAULT 10000	//送信完了待ちのタイムアウト時間をここに設定
/* PCLK=50[MHz]のとき、N=40,n=0にすると、ボーレートは38400[bps]になります。 */
/* PCLK=50[MHz]のとき、N=0,n=0にすると、ボーレートは1562500[bps]になります。 */









/***********************************************************************************************
	[README]	uartprint_v202　　【概要】　および　【使用方法】　【UART送信フォーマット設定方法】　【注意】

【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【　概要　】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】
・これは、RX62G マイコンのシリアル通信機能"SCI1"を用いて、外部に接続した機器と調歩同期式通信を行うライブラリです。
・ArduinoIDE のシリアルモニタ・シリアルプロッタ　その他 RS-232Cのテストツール等にて受信可能です。
・RX62G:"R5F562GAADFP" での使用を想定しています。
・ヒューマンエラー防止のため、スタンバイ解除の設定　および　割り込み、ポートに関する設定項目は含みません。セルフサービス♪ とします。





【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【　使用方法　】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】
＞ 【スタンバイ解除　および　割り込み許可，入出力設定】
＞	・SCI1 に関する割り込み要求許可ﾚｼﾞｽﾀを設定することにより、割り込みを許可してください。また、優先順位についても任意に設定してください。
＞	・ポートAデータディレクションﾚｼﾞｽﾀにSCIポートの入出力設定およびバッファの設定を行ってください。
＞	・モジュールストップコントロールﾚｼﾞｽﾀを設定することにより、SCI1モジュールのｽﾀﾝﾊﾞｲを解除してください。
＞	（例）
＞
＞		// SCIの割り込み設定   ちなみにこの書き方は、iodefine.hの説明欄に乗ってた気がする。
＞		IEN(SCI1,ERI1) = 1;		//割り込み要求許可ﾚｼﾞｽﾀ　p.246
＞		IEN(SCI1,RXI1) = 1;
＞		IEN(SCI1,TXI1) = 1;
＞		IEN(SCI1,TEI1) = 1;
＞		IPR(SCI1,ERI1) = 2;		//割り込み要因プライオリティﾚｼﾞｽﾀ　p.247
＞		IPR(SCI1,RXI1) = 2;		//割り込み優先順位 レベル:2　（数値大きいほど優先順位高い）　0:割込み禁止 max15まで
＞		IPR(SCI1,TXI1) = 2;
＞		IPR(SCI1,TEI1) = 2;
＞
＞		// 入出力設定 および入力バッファの適用
＞		PORTD.ICR.BYTE = 0xFF;	//入力バッファを有効にする。入力ﾊﾞｯﾌｧｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ SCIのところなので適用
＞		PORTD.DDR.BIT.B3 = 1;				//SCI1 TXD1
＞		PORTD.DDR.BIT.B5 = 0;				//SCI1 RXD1
＞
＞		// SCI1のスタンバイ解除
＞		SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;		//SCI1モジュール


＞【 送信割込みの配置】
＞	・ intprg.c に送信割込み・送信完了割込み関数を実装する必要があります。
＞	・ intprg.c の [SCI1 TXI項] および [SCI TEI項]に、 以下の例に従って関数を実装（関数名を記述）してください。
＞	・ 実装する関数は次の通りです。		TXI1 ... " UART1_TXI_interrupt(); "
＞								TEI1 ... " UART1_TEI_interrupt(); "
＞	（例）
＞
＞		// SCI1 TXI1
＞		void Excep_SCI1_TXI1(void){ UART1_TXI_interrupt(); }
＞
＞		// SCI1 TEI1
＞		void Excep_SCI1_TEI1(void){ UART1_TEI_interrupt(); }


＞【 設定関数の呼び出し】　UART1_TX_init();
＞	・SCI1 に関する設定を行う関数です。
＞	・これを初期設定ルーチン中に呼び出してください。呼び出し後に uartprintの機能（transmit等）が使用可能となります。

＞【 送信処理の呼び出し方法１】　UART1_transmit( 配列名　);
＞	・文字列の送信をおこなう関数です。　引数をchar型配列とするタイプです。
＞	・送信したいときに、char型配列を引数としてこの関数を呼び出してください。
＞	（例）　変数sensValueを送信したいとき
＞
＞		char mojiretu[64];								// char型の配列をバッファとして確保する。
＞		sprintf( mojiretu, "sensor:%d\n", sensValue );	// 上のバッファを第一引数として、sprintfにて書式を指定する。
＞		UART1_transmit( mojiretu );						// 上のバッファを引数として、transumitを実行する。
＞														// →→送信処理が行われます。

＞【 送信処理の呼び出し方法２】　UART1_vtransmit( 送信書式 );
＞	・文字列の送信を行う関数です。　引数をprintfと同じ書式で指定するタイプです。
＞	・送信したいときに、printfの要領でこの関数を呼び出してください。
＞	（例）　変数sensValueを送信したいとき
＞
＞		UART1_vtransmit( "sensor:%d\n", sensValue );	//
＞														// →→送信処理が行われます。








【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【　UART送信フォーマット設定方法　】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】
・UARTの送信フォーマットの設定は、ソースファイル "uartprint.c" にて記述します。
・uartprint.c　の上部に記載しているマクロにて、設定を変更することができます。
・ボーレートは、システムクロックおよび N, n の値に依存します。詳細はハードウェアマニュアルを参照してください。
		>>>	UART1_TXbufSize
				関数側での送信バッファとして確保する文字配列のサイズ[B]を指定してください。
		>>>	UART1_PARITY
				UARTのフォーマットとして、パリティﾋﾞｯﾄの有無を選択してください。
		>>>	UART1_PARITYMODE
				UARTのフォーマットとして、パリティビットの極性を選択してください。
				ただしパリティなしの場合は無視されます。
		>>>	UART1_STOPBIT
				UARTのフォーマットとして、ストップビットの長さを選択してください。
		>>>	UART1_N_Value
				ﾋﾞｯﾄﾚｰﾄの設定です。
				ﾊｰﾄﾞｳｪｱﾏﾆｭｱﾙの設定例などを参考に、ﾋﾞｯﾄﾚｰﾄﾚｼﾞｽﾀの値 N を入力してください。
		>>>	UART1_n_Value
				ビットレートの設定です。
				ﾊｰﾄﾞｳｪｱﾏﾆｭｱﾙの設定例などを参考に、クロック選択ビットの値 n を選択してください。
		>>>	TIMEOUT_COUNTER_DEFAULT
				大抵の場合変更しなくてよいと思います。 文字列がきちんと送信されない場合限定？
				送信処理のループから強制的に脱出させる時間(カウント数)を設定してください。
				whileの繰返し回数をカウントしますので、時間はｸﾛｯｸや割込み状況によって異なります。





【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【【　注意　】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】】
・RX62G / RX62T ハードウェアマニュアル 22章　シリアルコミュニケーションインタフェース（SCIb） に基づき設定することを前提とします。
・マルチプロセッサモードには対応していません。
・sprintf　の処理にはある程度の時間 (だいたい 20 ～ 100 us 程度以上） を要します。　メイン処理中での実行は推奨しません。
・100 Hz 程度の割り込みルーチン内での実行を推奨します。
・UART1_vtransmit では関数内に可変長引数のための処理を使用しています。気になる場合は素直にUART_transmitをご使用ください。
・UART1_vtransmit では関数内に sprintf を使用しています。処理に時間を要するので、実行する際は注意してください。
・処理時間が長くかかりすぎている場合、文字列が最後まで正しく送信されない、繰り返し処理に異常に時間を要する 等の症状が出現します。

・開発には、マイコンボードHSBRX62G（株式会社北斗電子）を使用しました
・2020年9月4日現在(version202)、初期設定を含め送信処理のみに対応しています。受信処理は後々追加していきます。
・2020年9月4日現在(version202)、SCI0およびSCI2の機能については未対応です。後々追加予定です。

・以下の関数を用いて、タイムアウトや送信エラー等のフラグを取得することができます。
（例）　それぞれの値を取得して変数 a,b,c に代入する場合

		a = getssr(void);		//シリアルステータスﾚｼﾞｽﾀの値を取得する関数。
		b = getstate(void);		//送信処理フラグを取得する関数。送信処理の途中だと１が返ってくるはず。
		c = gettimeout(void);	//タイムアウト判定結果を取得する関数。タイムアウトしてた場合は１が返ってくるはず。




uartprint version 202
作成者：射矢響
作成日：2020 09 04
更新日：--

************************************************************************************************/







#include "uartprint.h"		//UARTでprintfするためのライブラリ
#include "iodefine.h"		//このマイコンの使うためのライブラリ
#include "stdio.h"			//文字の入出力のためのライブラリ sprintfとか
#include "string.h"			//文字列の処理のためのライブラリ strlenとか


//グローバル変数
int UART1_copied_SSR=0;		//送信直後のｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀの値
int UART1_TXF = 0;			//送信作業中だと１
int UART1_TIMEOUT = 0;		//待ち処理でタイムアウト発生すると１
char UART1_TXbuf[UART1_TXBUF_SIZE];	//送信処理で参照されるﾊﾞｯﾌｧ の文字配列


/**　実行関数その１　文字列を受け取るタイプ　**/
void UART1_transmit(const char* InputAddress){
	int i=0;

	//送信処理始めますよフラグ
	UART1_TXF = 1;

	//ｿｰｽ内ﾊﾞｯﾌｧに文字をコピー　但しマクロで指定された配列の数だけ！配列数超過防止
	//InputAddress は文字配列の先頭のアドレスを示すポインタね
	while( InputAddress[i] != 0x00 && i < UART1_TXBUF_SIZE ){
		UART1_TXbuf[i] = InputAddress[i];
		i++;
	}

	//割り込み要求許可を変更すると、送信動作が始まる。
	SCI1.SCR.BIT.TEIE = 0;			//TE:送信完了割込み要求を禁止 ←ここは最終文字で許可する。
	ICU.IER[0x1B].BIT.IEN5 = 0;		//TE:送信完了割込み要求を禁止 ←こちらへ変更を書けと書いてあったりなかったり。
	SCI1.SCR.BIT.TIE = 1;			//TXI:送信割り込み要求を許可 データエンプティ
	ICU.IER[0x1B].BIT.IEN4 = 1;		//TXI:送信割り込み要求を許可
	SCI1.SCR.BIT.TE = 1;			//シリアル送信動作を許可
}

/**　実行関数その２　可変長引数を使ってprintfと同じ形式で受け取るタイプ 　**/
void UART1_vtransmit(const char *format, ...){
	char transmitBuf[64];
	va_list ptbuf1;
	va_start(ptbuf1, format);

	vsprintf( transmitBuf, format, ptbuf1 );	// va_startで変換された文字列を格納する
	va_end(ptbuf1);

	/* 送信処理は同じ */
	UART1_transmit( transmitBuf );
}


/**　送信処理　**/
#pragma interrupt(UART1_TXI_interrupt(vect=VECT(SCI1, TXI1)))
void UART1_TXI_interrupt(void){
	int i = 0;
	char TX_temp = 0;
	int TX_Len = 0;
	int timeout_counter = TIMEOUT_COUNTER_DEFAULT;
	static int TXcount = 0;

	///"TX割込みルーチンでTDRレジスタに送信データを1回だけ書いてください。" だそうです。
	//バッファに入った文字列の長さを取得
	TX_Len = strlen( UART1_TXbuf );

	//送信する１文字だけを取得
	TX_temp = UART1_TXbuf[ TXcount ];
	//↓書き込むｊとデータを送信。 IF文中で使うとおかしくなるのか？いやそういうわけではないっぽいか
	SCI1.TDR = TX_temp;				//トランスミットデータﾚｼﾞｽﾀに書き込み


	//文字を送る処理
	if( 0 <= TXcount && TXcount < TX_Len ){
		SCI1.SCR.BIT.TEIE = 0;			//TE:送信完了割込み要求を禁止
		ICU.IER[0x1B].BIT.IEN5 = 0;		//TE:送信完了割込み要求を禁止
		TXcount++;
	}


	//文字を送り終えた時の処理
	if( TX_Len == TXcount ){
		SCI1.SCR.BIT.TEIE = 1;			//TE:送信完了割込み要求を許可
		ICU.IER[0x1B].BIT.IEN5 = 1;		//TE:送信完了割込み要求を許可

		SCI1.SCR.BIT.TIE = 0;			//TXI:送信割り込み要求を禁止
		ICU.IER[0x1B].BIT.IEN4 = 0;		//TXI:送信割り込み要求を禁止
		while( SCI1.SSR.BIT.TEND == 0 && timeout_counter-- > 0 );//送信中のあいだは繰り返す。
		if( timeout_counter == 0 ){ UART1_TIMEOUT = 1; }
		SCI1.SCR.BIT.TE = 0;			//シリアル送信動作を禁止
		for( i=0; i<UART1_TXBUF_SIZE; i++ ){ UART1_TXbuf[i] = 0; }

		TXcount=0;

		//なぜか一番後ろに<NULL>が含まれてる。文字列の決まりなんですと。
	}
}





/**　送信完了処理　**/
#pragma interrupt(UART1_TEI_interrupt(vect=VECT(SCI1, TEI1)))
void UART1_TEI_interrupt(void){
	UART1_copied_SSR = SCI1.SSR.BYTE & 0xFF;

	SCI1.SCR.BIT.TEIE = 0;			//TE:送信完了割込み要求を禁止
	ICU.IER[0x1B].BIT.IEN5 = 0;		//TE:送信完了割込み要求を禁止

	UART1_TXF = 0;
}





/**　セットアップ関数　**/
void UART1_TX_init(void){
	int i;						//暇つぶしのためのカウンタ
	//Nの範囲は？nの範囲は？
	//タイムアウトとかは？
	//受信エラーフラグがセットされたままだと、送信できないと思います。

	//シリアルコントロールレジスタ　		→→調歩同期式に設定します。
	SCI1.SCR.BIT.TIE = 0;		//TX1割り込み要求を禁止
	SCI1.SCR.BIT.RIE = 0;		//RX1割り込み要求を禁止
	SCI1.SCR.BIT.TE = 0;		//シリアル送信動作を禁止
	SCI1.SCR.BIT.RE = 0;		//シリアル受信動作を禁止
	SCI1.SCR.BIT.MPIE = 0;		//通常の受信動作 ﾏﾙﾁﾌﾟﾛｾｯｻ割込許可ﾋﾞｯﾄ
	SCI1.SCR.BIT.TEIE = 0;		//TEI割り込み要求を禁止
	SCI1.SCR.BIT.CKE = 0;		//内臓ﾎﾞｰﾚｰﾄｼﾞｪﾈﾚｰﾀを使用 ｸﾛｯｸ許可ﾋﾞｯﾄ

	//シリアルモードレジスタ								→フォーマットはp.927の表の1行目
	SCI1.SMR.BIT.CM = 0;							//調歩同期式モード		ｺﾐｭﾆｹｰｼｮﾝﾓｰﾄﾞﾋﾞｯﾄ
	SCI1.SMR.BIT.CHR = 0;							//ﾃﾞｰﾀ長8bitで送受信 	ｷｬﾗｸﾀ長ﾋﾞｯﾄ
	SCI1.SMR.BIT.PE = UART1_PARITY & 1;				//ﾊﾟﾘﾃｨﾋﾞｯﾄなし 		パリティ許可ビット
	SCI1.SMR.BIT.PM = UART1_PARITYMODE & 1;			//偶数パリティ(使わん)	パリティモードビット
	SCI1.SMR.BIT.STOP =( UART1_STOPBIT - 1 )& 1;	//1ｽﾄｯﾌﾟﾋﾞｯﾄ 			ｽﾄｯﾌﾟﾋﾞｯﾄ長選択ﾋﾞｯﾄ
	SCI1.SMR.BIT.MP = 0;							//ﾏﾙﾁﾌﾟﾛｾｯｻ通信機能を禁止
	SCI1.SMR.BIT.CKS = UART1_n_VALUE & 3;					//p.921の「n」			クロック選択ビット

	//スマートカードモードレジスタ				//→使ってないけど、初期値を書いとかんといけんのかな？
	SCI1.SCMR.BYTE = 0xF2;				//初期値っす
	//ビットレートレジスタ						//→ここらへんの設定 p.918 らへん
	SCI1.BRR = UART1_N_VALUE & 0xFF;	//=Nです。コメントに記載してます。
	//少なくとも1ﾋﾞｯﾄ期間ひつまぶし
	for(i=0;i<5400;i++);		//ﾎﾞｰﾚｰﾄ38400bpsのときｼｽﾃﾑｸﾛｯｸ100MHzだと2700ｶｳﾝﾄ以上くらいかな。
	//ステータスﾚｼﾞｽﾀフラグ解除			//これはここに必要なのかな？受信フラグを0にしといてください。変わらんかったけど一応おいとこうか
	SCI1.SSR.BIT.ORER = 0;
	SCI1.SSR.BIT.FER = 0;
	SCI1.SSR.BIT.PER = 0;
	//シリアル拡張モードレジスタ
	SCI1.SEMR.BIT.RXDESEL = 0;	//初期値まま			エッジ検出選択ビット
	SCI1.SEMR.BIT.NFEN = 0;		//ノイズ除去機能無効	ノイズ除去選択ビット
	SCI1.SEMR.BIT.ABCS = 0;		//初期値まま			調歩同期基本クロック選択ビット



}


/**　状態を取得する関数　**/
int getssr(void){ return UART1_copied_SSR; }
int getstate(void){ return UART1_TXF; }
int gettimeout(void){ return UART1_TIMEOUT; }



