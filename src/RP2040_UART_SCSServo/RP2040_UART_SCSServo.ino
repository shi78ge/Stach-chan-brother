#define ELEMENTS_NUM 2 /**< カンマ区切りデータの項目数 */
#include <SoftwareSerial.h>
SoftwareSerial mySerial(21, 20); // RX, TX（ソフトウェアシリアル）
#include <SCServo.h>
SCSCL sc;

/*
 * シリアル通信で文字列を受け取る方法については以下の記事からコードを拝借させて頂きました
 * https://qiita.com/Ninagawa_Izumi/items/6a18249a31a87dad84e2
*/


uint16_t Time = 0;
uint8_t Speed = 1500;

/**
 * @brief       受信済み文字列を格納する配列
 */
static String elements[ELEMENTS_NUM];
static uint16_t value[ELEMENTS_NUM];
static int received_elements_num = 0; /**< 受信済み文字列の数 */

/**
 * @brief       セットアップ関数
 */
void setup()
{
  //↓モニタ用シリアル通信
  Serial.begin(115200); 
  
  //↓ESP32とのUART通信用
  Serial1.setRX(17); 
  Serial1.setTX(16);
  Serial1.begin(115200);

  //↓SCSサーボ駆動用
  mySerial.begin(1000000);
  sc.pSerial = &mySerial;
  
  delay(1000);
}

/**
 * @brief       ループ関数
 */
void loop()
{
    String line;              // 受信文字列
    unsigned int beginIndex;  // 要素の開始位置

    // シリアルモニタやProcessingから"AB,C,DEF,12,3,45,A1,2B,-1,+127"のように
    // ELEMENTS_NUM個の文字列の間にカンマを付けて送る
    // 送信側の改行設定は「LFのみ」にすること
    // シリアル通信で1行（改行コードまで）読み込む
    line = Serial1.readStringUntil('\n');

    beginIndex = 0;
    for (received_elements_num = 0; received_elements_num < ELEMENTS_NUM; received_elements_num++) {
        // 最後の要素ではない場合
        if (received_elements_num != (ELEMENTS_NUM - 1)) {
            // 要素の開始位置から，カンマの位置を検索する
            unsigned int endIndex;
            endIndex = line.indexOf(',', beginIndex);
            // カンマが見つかった場合
            if (endIndex != -1) {
                // 文字列を切り出して配列に格納する
                elements[received_elements_num] = line.substring(beginIndex, endIndex);
                // 要素の開始位置を更新する
                beginIndex = endIndex + 1;
            }
            // カンマが見つからなかった場合はfor文を中断する
            else {
                break;
            }
        }
        // 最後の要素の場合
        else {
            elements[received_elements_num] = line.substring(beginIndex);
        }
    }

    // 受信済み文字列の数がELEMENTS_NUM以上の場合
    if (received_elements_num >= ELEMENTS_NUM) {
        // 受信済み文字列をスラッシュ区切りでシリアルモニタに送信する
        for (int i = 0; i < ELEMENTS_NUM; i++) {
            value[i] = elements[i].toInt();
            Serial.print(value[i]);
            Serial.print("/");
        }
        Serial.println();
    }
    sc.WritePos(2, value[0], Time, Speed);
    sc.WritePos(1, value[1], Time, Speed);
    //delay(1);
}
