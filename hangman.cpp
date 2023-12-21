#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class GameWords
{
public:
    unordered_map<string, vector<string>> words_map;
    void addWords(string topic, const vector<string> array)
    {
        words_map[topic] = array;
    }
};

class HangMan
{
private:
    const int MAX_TRIES = 5;
    int misses = 0;
    string current_word = "";
    string topic = "";
    vector<int> found_indices;
public:
    void chooseTopic(GameWords& game_words)
    {
        for (const auto& pair : game_words.words_map)
        {
            cout << "topic: " << pair.first << std::endl;
        }
        while (true)
        {
            cout << "Choose a topic: ";
            cin >> topic; cout << endl;
            if (topic.size() && !game_words.words_map.count(topic))
            {
                cout << "Choose a valid topic!" << endl;
                topic = "";
            }
            else
            {
                cout << "you chose: " << topic << endl;
                break;
            }
        }
        getWord(game_words);
    }

    void getWord(GameWords& game_words)
    {
        const int INDEX = rand()%game_words.words_map[topic].size();
        current_word = game_words.words_map[topic][INDEX];
    }

    bool duplicateChar(const char user_guess)
    {
        for (int i = 0; i < found_indices.size(); i++)
        {
            int index = found_indices[i];
            if (current_word[index] == user_guess)
            {
                cout << "Already picked this character" << endl;
                misses --;
                return false;
            }
        }
        return true;
    }

    bool isValid(const char user_guess)
    {
        bool valid = false;
        if (!duplicateChar(user_guess))return false;
        for (int i = 0; i < current_word.size(); i++)
        {
            if (current_word[i] == user_guess)
            {
                found_indices.push_back(i);
                valid = true;
            }
        }
        return valid;
    }

    bool hasWon()
    {
        string result = "";
        for (int i = 0; i < current_word.size(); i++)
        {
            if (!isalnum(current_word[i]))
            {
                result += current_word[i];
                continue;
            }
            result += '_';
            for (int j = 0; j < found_indices.size(); j++)
            {
                int index = found_indices[j];
                if (index == i)
                    result[i] = current_word[i]; 
            }
        }
        cout << result << endl;
        return result == current_word;
    }

    void reset()
    {
        char answer;
        while (!answer)
        {
            cout << "Continue playing? [y/n] ";
            cin >> answer;
            switch(answer)
            {
                case('n'):
                {
                    cout << "Goodbye." << endl;
                    exit(0);
                }
                case('y'):
                {
                    topic = "";
                    misses = 0;
                    found_indices = {};
                    break;
                }
                default:
                {
                    answer = 0;
                    cout << "Choose a valid answer" << endl; 
                }
            }
        }
        cout << "\033[2J\033[H";
    }

    void GAME(GameWords& game_words)
    {
        char user_guess;
        while (misses < MAX_TRIES)
        {
            if (!topic.size())
                chooseTopic(game_words);
            cout << "choose a letter: ";
            cin >> user_guess; cout << endl;
            try
            {   
                if (!isValid(user_guess))
                {
                    misses++;
                    cout << "Tries left: " << MAX_TRIES-misses << endl;
                }
            }
            catch(const std::exception& e)
            {
                cout << "Choose a valid character (a - z) !" << endl;
            }
            if (hasWon())
            {
                cout << "NOT BAD NOT BAD,YOU WON! :) " << endl;
                reset();
            }
            if (misses == MAX_TRIES)
            {
                cout << current_word << endl;
                cout << "Oooh is that all you can do, haha :p" << endl;
                reset();
            }
        }
    }
};

int main()
{
    srand(time(NULL));
    GameWords game_words;
    HangMan hang_man;
    vector<string> movies = {
        "avengers", 
        "pokemon the first movie",
        "kung fu panda",
        "wolverine",
        "five night's at freddy's"
    };
    vector<string> countries = {
        "north america",
        "europe",
        "china",
        "south korea",
        "cananda"
    };
    vector<string> foods = {
        "pizza",
        "apples",
        "fish",
        "steak"
    };
    game_words.addWords("Movies", movies);
    game_words.addWords("Countries", countries);
    game_words.addWords("Foods", foods);
    hang_man.GAME(game_words);
}