
# game.py

from typing import Optional
from board import ChessBoard
from player import Player
from utils import PieceType

class Game:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(Game, cls).__new__(cls)
            cls._instance._initialize()
        return cls._instance

    def _initialize(self):
        self.player1: Optional[Player] = None
        self.player2: Optional[Player] = None
        self.board: ChessBoard = ChessBoard.get_instance()
        self.current_player: Optional[Player] = None
        self.is_game_over: bool = False
        self.turn_count: int = 1

    @classmethod
    def get_instance(cls):
        return cls()

    def initialize_game(self, player1_name: str, player2_name: str):
        self.player1 = Player(player1_name)
        self.player2 = Player(player2_name)
        self.current_player = self.player1
        self.is_game_over = False
        self.turn_count = 1
        print(f"Game initialized. {player1_name} starts.")

    def restart_game(self):
        if self.player1 and self.player2:
            player1_name = self.player1.get_name()
            player2_name = self.player2.get_name()
            self.board.clear_board()
            self.initialize_game(player1_name, player2_name)
            print("Game has been restarted.")
        else:
            print("Game not initialized. Cannot restart.")

    def switch_player(self):
        if self.current_player == self.player1:
            self.current_player = self.player2
        else:
            self.current_player = self.player1
            self.turn_count += 1

    def check_game_over(self) -> bool:
        player1_queen_surrounded = False
        player2_queen_surrounded = False

        # Find QueenBee positions and check if surrounded
        for x in range(self.board.get_width()):
            for y in range(self.board.get_height()):
                piece = self.board.get_piece_at(x, y)
                if piece and piece.get_piece_type() == PieceType.QUEEN_BEE:
                    if piece.get_owner() == self.player1:
                        player1_queen_surrounded = self.board.is_position_surrounded(x, y)
                    elif piece.get_owner() == self.player2:
                        player2_queen_surrounded = self.board.is_position_surrounded(x, y)

        if player1_queen_surrounded or player2_queen_surrounded:
            self.is_game_over = True
            print("Game Over!")

            if player1_queen_surrounded and player2_queen_surrounded:
                print("It's a draw!")
            elif player1_queen_surrounded:
                print("Player 2 wins!")
            else:
                print("Player 1 wins!")
            return True
        return False

    def get_winner(self) -> Optional[Player]:
        if self.is_game_over:
            player1_queen_surrounded = False
            player2_queen_surrounded = False

            for x in range(self.board.get_width()):
                for y in range(self.board.get_height()):
                    piece = self.board.get_piece_at(x, y)
                    if piece and piece.get_piece_type() == PieceType.QUEEN_BEE:
                        if piece.get_owner() == self.player1:
                            player1_queen_surrounded = self.board.is_position_surrounded(x, y)
                        elif piece.get_owner() == self.player2:
                            player2_queen_surrounded = self.board.is_position_surrounded(x, y)

            if player1_queen_surrounded and not player2_queen_surrounded:
                return self.player2
            elif player2_queen_surrounded and not player1_queen_surrounded:
                return self.player1
            else:
                return None  # Draw
        return None

    def play_turn(self):
        if self.is_game_over:
            print("Game is already over.")
            return

        print(f"{self.current_player.get_name()}'s turn.")

        valid_input = False
        while not valid_input:
            action = input("Enter action (P for place, M for move): ").strip().upper()

            try:
                if action == 'P':
                    x, y, piece_type_str = input("Enter coordinates (x y) and piece type (0-7): ").split()
                    x, y, piece_type = int(x), int(y), int(piece_type_str)
                    self.current_player.place_piece(piece_type, x, y, self.board)
                    valid_input = True
                elif action == 'M':
                    x, y, to_x, to_y = map(int, input("Enter from coordinates (x y) and to coordinates (toX toY): ").split())
                    piece_name = input("Enter piece name to move (QueenBee, Beetle, Spider, Ant, Grasshopper, Ladybug, Mosquito, Pillbug): ").strip()
                    self.current_player.move_piece(x, y, to_x, to_y, piece_name, self.board)
                    valid_input = True
                else:
                    print("Invalid action. Please enter 'P' for place or 'M' for move.")
            except (ValueError, IndexError, RuntimeError) as e:
                print(f"Invalid move: {e}")
                print("Please try again.")

        self.display_board()
        self.display_turn_count()

        if self.check_game_over():
            winner = self.get_winner()
            if winner:
                print(f"{winner.get_name()} wins the game!")
            else:
                print("The game ended in a draw!")
            return

        self.switch_player()

    def display_board(self):
        self.board.display_board()

    def display_turn_count(self):
        print(f"Current Turn: {self.turn_count}")

    def end_game(self):
        self.is_game_over = True
        print("Game has ended.")

    def get_current_player(self) -> Optional[Player]:
        return self.current_player


