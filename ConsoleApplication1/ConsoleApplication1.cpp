#include <iostream>
#include <Windows.h>

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
		std::cout << "Z: クリア, X: ゲームオーバー\n";
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
	// シーン: 0=Title, 1=Play, 2=GameOver, 3=GameClear
	int scene = 0;
	bool running = true;

	// 起動時にタイトル表示
	GameManager::Instance().GameTitle();

	while (running) {
		// タイトル -> スタート
		if (scene == 0) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				GameManager::Instance().GameGamen();
				scene = 1;
				
			}
		}
		// プレイ中の入力（Z: クリア, X: オーバー）
		else if (scene == 1) {
			if (GetAsyncKeyState('Z') & 0x8000) {
				GameManager::Instance().GameClear();
				scene = 3;
				
			}
			else if (GetAsyncKeyState('X') & 0x8000) {
				GameManager::Instance().GameOver();
				scene = 2;
				
			}
		}
		// 結果画面 -> タイトルへ
		else if (scene == 2 || scene == 3) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				GameManager::Instance().GameTitle();
				scene = 0;
				
			}
		}

		// 終了（Esc）
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			running = false;
			
		}

		
	}

	return 0;
}
