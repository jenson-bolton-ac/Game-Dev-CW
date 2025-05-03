#include "GameStateManager.h"
#include <algorithm>

GameStateManager::GameStateManager()
    : mCurrentState(GameState::START_MENU), mNextListenerId(1)
{}

GameStateManager::~GameStateManager() {}

GameState GameStateManager::GetState() const {
    return mCurrentState;
}

void GameStateManager::SetState(GameState newState) {
    if (newState == mCurrentState) return;
    mCurrentState = newState;
    // Notify all listeners
    for (auto& entry : mListeners) {
        entry.second(mCurrentState);
    }
}

int GameStateManager::AddListener(const Listener& listener) {
    int id = mNextListenerId++;
    mListeners.emplace_back(id, listener);
    return id;
}

void GameStateManager::RemoveListener(int listenerId) {
    mListeners.erase(
        std::remove_if(
            mListeners.begin(), mListeners.end(),
            [listenerId](const std::pair<int, Listener>& entry) {
                return entry.first == listenerId;
            }
        ),
        mListeners.end());
}
