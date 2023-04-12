// DiyKeypadListener.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <tchar.h>

using namespace std;

int main() {
    // シリアルポートの設定
    HANDLE hSerial = CreateFile(_T("COM3"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        cout << "シリアルポートのオープンに失敗しました。" << endl;
        return 1;
    }
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        cout << "シリアルポートの設定取得に失敗しました。" << endl;
        CloseHandle(hSerial);
        return 1;
    }
    dcbSerialParams.BaudRate = CBR_9600; // ボーレートの設定
    dcbSerialParams.ByteSize = 8; // データビットの設定
    dcbSerialParams.Parity = NOPARITY; // パリティビットの設定
    dcbSerialParams.StopBits = ONESTOPBIT; // ストップビットの設定

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        cout << "シリアルポートの設定変更に失敗しました。" << endl;
        CloseHandle(hSerial);
        return 1;
    }

    //モニター設定
    //モニターのハンドル取得
    HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
    DWORD nowBrightness = 0;

    // シリアル通信の読み書き
    char szBuff[256] = { 0 };
    DWORD dwBytesRead = 0;
    /*DWORD dwBytesWritten = 0;
    string strMessage = "Hello, Arduino!";
    if (!WriteFile(hSerial, strMessage.c_str(), strMessage.length(), &dwBytesWritten, NULL)) {
        cout << "データの送信に失敗しました。" << endl;
        CloseHandle(hSerial);
        return 1;
    }*/

    while (true) {
        if (!ReadFile(hSerial, szBuff, 10, &dwBytesRead, NULL)) {
            cout << "データの受信に失敗しました。" << endl;
            CloseHandle(hSerial);
            return 1;
        }
        cout << "受信データ: " << szBuff << endl;
        
        memset(szBuff, 0, sizeof(szBuff));
        
        nowBrightness++;
        cout << nowBrightness << endl;
        //SetMonitorBrightness(hMonitor, nowBrightness);
    }
    

    // シリアルポートのクローズ
    CloseHandle(hSerial);

    cout << "おわり" << endl;

    return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
