from kipy import KiCad
from kipy.geometry import Vector2
from kipy.board import BoardLayer, Board
from kipy.board_types import Track, Via

import math

# Constants
MM_TO_NM = 1_000_000  # KiCad expects positions and lengths in nanometers

def mm(value: float) -> int:
    """Convert mm to nm (as integer) for KiCad."""
    return int(value * MM_TO_NM)

# Spiral generator
def generate_spiral(center_mm, start_radius_mm, spacing_mm, turns, segments_per_turn, outward=True):
    points = []
    for i in range(turns * segments_per_turn):
        t = i if outward else (turns * segments_per_turn - i - 1)
        theta = t * 2 * math.pi / segments_per_turn
        r = start_radius_mm + spacing_mm * theta / (2 * math.pi)
        x_mm = center_mm[0] + r * math.cos(theta)
        y_mm = center_mm[1] + r * math.sin(theta)
        points.append(Vector2.from_xy(mm(x_mm), mm(y_mm)))
    return points

def generate_double_spiral(center_mm, start_radius_mm, spacing_mm, turns, segments_per_turn):
    # First spiral: front copper
    points_front = generate_spiral(center_mm, start_radius_mm, spacing_mm, turns, segments_per_turn, outward=True)
    for a, b in zip(points_front, points_front[1:]):
        track = Track()
        track.layer = BoardLayer.BL_F_Cu
        track.width = track_width_nm
        track.start = a
        track.end = b
        board.create_items(track)
    board.save()
    # Via at the outer end
    end_point = points_front[0]
    via = Via()
    via.position = end_point
    via.diameter = mm(0.45)
    via.drill = mm(0.3)
    via.layers = [BoardLayer.BL_F_Cu, BoardLayer.BL_B_Cu]
    board.create_items(via)

    # Second spiral: back copper, inward
    points_back = generate_spiral(center_mm, start_radius_mm, spacing_mm, turns, segments_per_turn, outward=False)
    for a, b in zip(points_back, points_back[1:]):
        track = Track()
        track.layer = BoardLayer.BL_B_Cu
        track.width = track_width_nm
        track.start = a
        track.end = b
        board.create_items(track)

if __name__ == '__main__':
    # Connect to running KiCad instance
    kicad = KiCad()
    board = kicad.get_board()

    # Parameters
    center_mm = (100, 100)
    spacing_mm = 0.3
    turns = 8
    segments_per_turn = 30
    track_width_mm = 0.09
    track_width_nm = mm(track_width_mm)

    coil1_start_radius_mm = 0.25
    coil1_end_radius_mm = 17.78/2
    coil1_turns = int((coil1_end_radius_mm - coil1_start_radius_mm)/spacing_mm)

    coil2_start_radius_mm = 19.05/2
    coil2_end_radius_mm = 36.83/2
    coil2_turns = int((coil2_end_radius_mm - coil2_start_radius_mm)/spacing_mm)

    coil3_start_radius_mm = 38.1/2
    coil3_end_radius_mm = 55.88/2
    coil3_turns = int((coil3_end_radius_mm - coil3_start_radius_mm)/spacing_mm)

    generate_double_spiral(center_mm,coil1_start_radius_mm,spacing_mm, coil1_turns, segments_per_turn)
    board.save()
    generate_double_spiral(center_mm,coil2_start_radius_mm,spacing_mm, coil2_turns, segments_per_turn)
    board.save()
    generate_double_spiral(center_mm,coil3_start_radius_mm,spacing_mm, coil3_turns, segments_per_turn)
    board.save()

    print("✅ Spiral trace added to board on F.Cu")
