
# board.py

from typing import List, Optional
from utils import BOARD_SIZE, DIRECTIONS, PieceType
from piece import Piece, QueenBee, Beetle, Grasshopper, Spider, Ant, Ladybug, Mosquito, Pillbug

class ChessBoard:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(ChessBoard, cls).__new__(cls)
            cls._instance._initialize()
        return cls._instance

    def _initialize(self):
        # board is a 3D list: board[x][y] is a list of pieces (stack)
        self.board: List[List[List[Piece]]] = \
            [[[] for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]

    @classmethod
    def get_instance(cls):
        return cls()

    def get_width(self) -> int:
        return BOARD_SIZE

    def get_height(self) -> int:
        return BOARD_SIZE

    def is_within_bounds(self, x: int, y: int) -> bool:
        return 0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE

    def get_piece_at(self, x: int, y: int) -> Optional[Piece]:
        if not self.is_within_bounds(x, y) or not self.board[x][y]:
            return None
        return self.board[x][y][-1]  # Get the top piece of the stack

    def get_adjacent_pieces(self, x: int, y: int) -> List[Piece]:
        adjacent_pieces = []
        for dx, dy in DIRECTIONS:
            adj_x, adj_y = x + dx, y + dy
            if self.is_within_bounds(adj_x, adj_y):
                piece = self.get_piece_at(adj_x, adj_y)
                if piece:
                    adjacent_pieces.append(piece)
        return adjacent_pieces

    def has_adjacent_piece(self, x: int, y: int) -> bool:
        for dx, dy in DIRECTIONS:
            adj_x, adj_y = x + dx, y + dy
            if self.is_within_bounds(adj_x, adj_y) and self.board[adj_x][adj_y]:
                return True
        return False

    def is_position_surrounded(self, x: int, y: int, consider_edge: bool = False) -> bool:
        for dx, dy in DIRECTIONS:
            adj_x, adj_y = x + dx, y + dy
            if self.is_within_bounds(adj_x, adj_y):
                if not self.board[adj_x][adj_y]:  # If any adjacent position is empty
                    return False
            elif not consider_edge:  # If not considering edge as surrounded and out of bounds
                return False
        return True  # All adjacent positions are occupied or out of bounds (and consider_edge is True)

    def place_piece(self, x: int, y: int, piece_type: int, player):
        if not self.is_within_bounds(x, y):
            raise IndexError("Coordinates out of bounds.")

        new_piece: Optional[Piece] = None
        if piece_type == PieceType.QUEEN_BEE:
            new_piece = QueenBee(x, y, player)
        elif piece_type == PieceType.BEETLE:
            new_piece = Beetle(x, y, player)
        elif piece_type == PieceType.GRASSHOPPER:
            new_piece = Grasshopper(x, y, player)
        elif piece_type == PieceType.SPIDER:
            new_piece = Spider(x, y, player)
        elif piece_type == PieceType.ANT:
            new_piece = Ant(x, y, player)
        elif piece_type == PieceType.LADYBUG:
            new_piece = Ladybug(x, y, player)
        elif piece_type == PieceType.MOSQUITO:
            new_piece = Mosquito(x, y, player)
        elif piece_type == PieceType.PILLBUG:
            new_piece = Pillbug(x, y, player)
        else:
            raise ValueError("Invalid piece type.")

        # If the position is empty or the new piece is a Beetle, place it.
        # Otherwise, raise an error (cannot place on top of other pieces unless it\'s a Beetle).
        if not self.board[x][y] or new_piece.get_piece_type() == PieceType.BEETLE:
            self.board[x][y].append(new_piece)
        else:
            raise RuntimeError("Cannot place piece here. Position is occupied.")

    def move_piece(self, from_x: int, from_y: int, to_x: int, to_y: int, piece_name: str, player):
        if not self.is_within_bounds(from_x, from_y) or not self.is_within_bounds(to_x, to_y):
            raise IndexError("Coordinates out of bounds.")

        if not self.board[from_x][from_y]:
            raise ValueError("No piece at the starting position.")

        piece_to_move = self.board[from_x][from_y][-1]

        if piece_to_move.get_name() != piece_name:
            raise ValueError(f"No {piece_name} at the given position.")

        if piece_to_move.get_owner() != player:
            raise ValueError("You can only move your own pieces.")

        if not piece_to_move.is_valid_move(to_x, to_y, self):
            raise ValueError("Invalid move for this piece type.")

        # Remove the piece from the old position
        self.board[from_x][from_y].pop()

        # Update the piece\'s position and place it at the new position
        piece_to_move.set_position(to_x, to_y)
        self.board[to_x][to_y].append(piece_to_move)

    def display_board(self):
        for y in range(BOARD_SIZE):
            for x in range(BOARD_SIZE):
                piece = self.get_piece_at(x, y)
                if piece:
                    print(f"{piece.get_name()[0]}{piece.get_owner().get_name()[0]} ", end="")
                else:
                    print(".  ", end="")
            print()

    def clear_board(self):
        self.board = [[[] for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]
        print("The board has been cleared.")


