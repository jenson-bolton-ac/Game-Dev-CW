#ifndef __GAME_STATE_MANAGER_H__
#define __GAME_STATE_MANAGER_H__

#include <vector>
#include <functional>

/**
 * Represents the various states of the game.
 */
enum class GameState {
    START_MENU,
    PLAYING,
    GAME_OVER,
    SHOWING_HIGHSCORES
};

/**
 * Manages the current game state and notifies registered listeners of any changes.
 */
class GameStateManager {
public:
    using Listener = std::function<void(GameState)>;

    GameStateManager();
    ~GameStateManager();

    /**
     * Retrieves the current state.
     */
    GameState GetState() const;

    /**
     * Changes to a new state and notifies listeners.
     */
    void SetState(GameState newState);

    /**
     * Registers a listener to be notified when state changes.
     * Returns a listener ID for unregistration.
     */
    int AddListener(const Listener& listener);

    /**
     * Unregisters a previously added listener by its ID.
     */
    void RemoveListener(int listenerId);

private:
    GameState mCurrentState;
    int mNextListenerId;
    std::vector<std::pair<int, Listener>> mListeners;
};

#endif