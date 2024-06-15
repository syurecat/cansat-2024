#include <SoftwareSerial.h>

SoftwareSerial MWSerial(2, 3); // RX, TX

#define BLOCK_MAX 10

char *p;
char buf[256];

// データ列を区切り文字で分割してString型の配列に入れる。
int split( char* source, char delimiter, String* result ){
    char* p = source;
    char tmp[81];
    memset(tmp, 0, sizeof(tmp));
    int n = 0;
    int block = 0;

    // ヌル文字が来るか分けたブロック数がBLOCK_MAXになったらやめる。
    while( *p != 0 && block < BLOCK_MAX ){

        // 区切り文字が来たら、String型に変換する。
        if( *p == delimiter ){
            // 分割したデータが0バイトの時は何もしない。
            if( n > 0 ){
                // 念のためヌル文字を末尾に代入。
                tmp[n] = 0;
                result[block] = String(tmp);
                block++;

                n = 0;
                memset(tmp, 0, sizeof(tmp));
            }
        }else{
            tmp[n] = *p;
            n++;
        }
        p++;
    }

        return block;
}

void setup() {
    Serial.begin(38400);
    MWSerial.begin(38400);

    p = buf;
    memset(buf, 0, sizeof(buf));
}

void loop() {
    while (MWSerial.available()) {
        char before = *p;
        *p = MWSerial.read();

        if( before=='\r' && *p=='\n' ){
            *p = 0;                  // 末尾をNull文字にする
            String str[BLOCK_MAX];   // 区切り文字で分割した文字列を入れる
            int len = p-buf;         // シリアルで読み込んだデータ量の計算
            int block = split( buf, ';', str );  // データの分割

            if( block == 3 ){
                Serial.print("Message: ");
                Serial.println(buf);
                Serial.print("Serial No.: ");
                Serial.println(str[0]);
                Serial.print("LQI: ");
                Serial.println(str[1]);
                Serial.print("Data: ");
                Serial.println(str[2]);
                Serial.println();
            }

            p = buf;
            memset(buf, 0, sizeof(buf));
        }else{
            if( *p >= 0x20 ){
                p++;
            }
        }
    }

    while(Serial.available()){
        MWSerial.write(Serial.read());
    }
}