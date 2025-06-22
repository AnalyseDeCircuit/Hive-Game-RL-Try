
# player.py

from typing import Dict, Tuple
from board import ChessBoard
from utils import PieceType

class Player:
    DLC_PIECE_COUNT = {
        "QueenBee": 1, "Beetle": 2, "Spider": 2, "Ant": 3,
        "Grasshopper": 3, "Ladybug": 1, "Mosquito": 1, "Pillbug": 1
    }

    NO_DLC_PIECE_COUNT = {
        "QueenBee": 1, "Beetle": 2, "Spider": 2, "Ant": 3, "Grasshopper": 3
    }

    def __init__(self, name: str, use_dlc: bool = False):
        self.name = name
        self.piece_count: Dict[str, int] = \
            dict(Player.DLC_PIECE_COUNT) if use_dlc else dict(Player.NO_DLC_PIECE_COUNT)
        self.is_queen_bee_placed = False
        self.queen_bee_position: Tuple[int, int] = (-1, -1)

    def get_name(self) -> str:
        return self.name

    def _validate_piece_placement(self, piece_name: str):
        if piece_name not in self.piece_count or self.piece_count[piece_name] <= 0:
            raise RuntimeError(f"No {piece_name} pieces left to place.")

    def place_piece(self, piece_type: int, x: int, y: int, board: ChessBoard):
        piece_name = self._get_piece_name_from_type(piece_type)
        self._validate_piece_placement(piece_name)

        board.place_piece(x, y, piece_type, self)

        self.piece_count[piece_name] -= 1
        if piece_name == "QueenBee":
            self.is_queen_bee_placed = True
            self.queen_bee_position = (x, y)

    def move_piece(self, from_x: int, from_y: int, to_x: int, to_y: int, piece_name: str, board: ChessBoard):
        if not self.is_queen_bee_placed:
            raise RuntimeError("You must place the QueenBee before moving other pieces.")

        board.move_piece(from_x, from_y, to_x, to_y, piece_name, self)

        if piece_name == "QueenBee":
            self.queen_bee_position = (to_x, to_y)

    def use_special_ability(self, ability_name: str, x: int, y: int, board: ChessBoard, target_x: int = -1, target_y: int = -1):
        if ability_name == "Mosquito":
            self._use_mosquito_ability(x, y, board)
        # elif ability_name == "Tideworm": # Tideworm is not in the C++ code, skipping for now
        #     self._use_tideworm_ability(x, y, target_x, target_y, board)
        # elif ability_name == "Ladybug": # Ladybug move is handled in board.move_piece
        #     self._move_ladybug(x, y, target_x, target_y, board)
        else:
            raise ValueError("Invalid special ability.")

    def _use_mosquito_ability(self, mosquito_x: int, mosquito_y: int, board: ChessBoard):
        mosquito = board.get_piece_at(mosquito_x, mosquito_y)
        if not mosquito or mosquito.get_name() != "Mosquito":
            raise RuntimeError("No Mosquito at the specified position.")

        adjacent_pieces = board.get_adjacent_pieces(mosquito_x, mosquito_y)
        if not adjacent_pieces:
            raise RuntimeError("No adjacent pieces to mimic.")

        # Mimic the first adjacent piece's type
        chosen_piece = adjacent_pieces[0]
        mosquito.set_piece_type(chosen_piece.get_piece_type())

    def display_piece_count(self):
        print(f"Piece count for {self.name}:")
        for piece, count in self.piece_count.items():
            print(f"  {piece}: {count}")

    def get_queen_bee_position(self) -> Tuple[int, int]:
        if not self.is_queen_bee_placed:
            raise RuntimeError("QueenBee has not been placed.")
        return self.queen_bee_position

    def _get_piece_name_from_type(self, piece_type: int) -> str:
        if piece_type == PieceType.QUEEN_BEE:
            return "QueenBee"
        elif piece_type == PieceType.BEETLE:
            return "Beetle"
        elif piece_type == PieceType.SPIDER:
            return "Spider"
        elif piece_type == PieceType.ANT:
            return "Ant"
        elif piece_type == PieceType.GRASSHOPPER:
            return "Grasshopper"
        elif piece_type == PieceType.LADYBUG:
            return "Ladybug"
        elif piece_type == PieceType.MOSQUITO:
            return "Mosquito"
        elif piece_type == PieceType.PILLBUG:
            return "Pillbug"
        else:
            raise ValueError("Invalid piece type.")


