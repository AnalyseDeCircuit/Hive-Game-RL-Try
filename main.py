
# main.py

from game import Game
import os

def clear_screen():
    os.system("cls" if os.name == "nt" else "clear")

def display_main_menu():
    clear_screen()
    print("\n--- Hive Game ---")
    print("1. Start Game")
    print("2. Exit Game")
    return input("Choose an option: ").strip()

def get_player_names():
    player1_name = input("Enter Player 1's name: ").strip()
    player2_name = input("Enter Player 2's name: ").strip()
    return player1_name, player2_name

def game_loop(game: Game):
    exit_game_loop = False
    while not exit_game_loop:
        clear_screen()

        game.display_turn_count()

        current_player = game.get_current_player()
        if current_player:
            print(f"Current Player: {current_player.get_name()}")
            print("Player's Hand (Remaining Pieces):")
            current_player.display_piece_count()

        game.display_board()

        print("\n--- Game Menu ---")
        print("1. Play Turn")
        print("2. Restart Game")
        print("3. End Current Game")
        print("4. Return to Main Menu")
        choice = input("Choose an option: ").strip()

        if choice == '1':
            game.play_turn()
        elif choice == '2':
            game.restart_game()
        elif choice == '3':
            game.end_game()
            exit_game_loop = True # End current game and return to main menu
        elif choice == '4':
            exit_game_loop = True # Return to main menu
        else:
            print("Invalid option. Please choose a number between 1 and 4.")

        if not exit_game_loop:
            input("\nPress Enter to continue...")

def main():
    game = Game.get_instance()
    
    while True:
        main_menu_choice = display_main_menu()

        if main_menu_choice == '1':
            player1_name, player2_name = get_player_names()
            game.initialize_game(player1_name, player2_name)
            game_loop(game)
        elif main_menu_choice == '2':
            print("Exiting game. Goodbye!")
            break
        else:
            print("Invalid option. Please choose 1 or 2.")
            input("\nPress Enter to continue...")

if __name__ == "__main__":
    main()


