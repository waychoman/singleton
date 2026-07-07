#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

class GameManager {
private:
	GameManager() {}

public:

	//コピー代入の禁止
	GameManager(const GameManager&) = delete;

	GameManager& operator=(const GameManager&) = delete;

	static GameManager& Instance() {
		static GameManager instance; //c++11以降のスレッドセーフなローカルstatic
		return instance;
	}

	void GameTitle() {
		// ゲームタイトル表示処理
		std::cout << "=== タイトル ===\n";
		std::cout << "Space: スタート\n";
	}

	void GameGamen() {
		// ゲーム画面表示処理
		std::cout << "=== ゲーム画面 ===\n";
		std::cout << "A または D を選択してください（当たり 50% / ハズレ 50%）\n";
		std::cout << "目的: 当たりを13回連続で出すとクリア (1/8192)\n";
		std::cout << "A = 左, D = 右\n";
	}
	void GameOver() {
		// ゲームオーバー表示処理
		std::cout << "=== ゲームオーバー ===\n";
		std::cout << "Space: タイトルに戻る\n";
	}
	void GameClear() {
		// ゲームクリア表示処理
		std::cout << "=== ゲームクリア ===\n";
		std::cout << "Space: タイトルに戻る\n";
	}
};

// 使用例
int main() {
	// 乱数初期化
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	// シーン: 0=Title, 1=Play, 2=GameOver, 3=GameClear
	int scene = 0;
	bool running = true;
	int consecutiveHits = 0; // 連続当たり回数

	// 起動時にタイトル表示
	GameManager::Instance().GameTitle();

	while (running) {
		// タイトル -> スタート
		if (scene == 0) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				consecutiveHits = 0;
				GameManager::Instance().GameGamen();
				scene = 1;
				Sleep(150); // 簡易デバウンス
			}
		}
		// プレイ中: A または D で選択（ランダムで正解側を決める、プレイヤーが当てれば当たり）
		else if (scene == 1) {
			if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState('D') & 0x8000) {
				int player = (GetAsyncKeyState('A') & 0x8000) ? 0 : 1; // A=0, D=1
				int correct = std::rand() % 2; // 0 or 1 が正解側
				if (player == correct) {
					++consecutiveHits;
					// 確率表示: 1 / (2^consecutiveHits)
					int denom = 1 << consecutiveHits; // 2^n（consecutiveHits <= 13 なので安全）
					std::cout << "当たり! 連続 " << consecutiveHits << " 回 (確率: 1/" << denom << ")\n";
					if (consecutiveHits >= 13) {
						GameManager::Instance().GameClear();
						scene = 3;
					}
				}
				else {
					// ハズレ -> ゲームオーバー
					std::cout << "ハズレ...\n";
					consecutiveHits = 0;
					GameManager::Instance().GameOver();
					scene = 2;
				}
				Sleep(150); // デバウンス
			}
		}
		// 結果画面 -> タイトルへ
		else if (scene == 2 || scene == 3) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				GameManager::Instance().GameTitle();
				scene = 0;
				Sleep(150);
			}
		}

		// 終了（Esc）
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			running = false;
			Sleep(150);
		}

		// 軽い待ち（CPU負荷低減）
		Sleep(10);
	}

	return 0;
}
