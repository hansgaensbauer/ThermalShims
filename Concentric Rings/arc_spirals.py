# from kipy import KiCad
# from kipy.geometry import Vector2
# from kipy.board import BoardLayer
# from kipy.board_types import BoardArc, Via
# import math

# def mm(x: float) -> int:
#     return int(x * 1_000_000)

# def generate_spiral_arcs(center_mm, start_radius_mm, spacing_mm, turns, arcs_per_turn, outward=True):
#     """
#     Generates parameters for arcs approximating an Archimedean spiral.
#     Returns a list of dicts with arc center, radius, start_angle, sweep_angle (deg).
#     """
#     arcs = []
#     total_arcs = turns * arcs_per_turn
#     angle_per_arc = 360 / arcs_per_turn
#     for i in range(total_arcs):
#         t = i if outward else (total_arcs - i - 1)
#         start_angle = t * angle_per_arc
#         sweep_angle = angle_per_arc
#         # radius at start and end of arc segment
#         r_start = start_radius_mm + spacing_mm * math.radians(start_angle) / (2 * math.pi)
#         r_end = start_radius_mm + spacing_mm * math.radians(start_angle + sweep_angle) / (2 * math.pi)
#         # approximate radius as average (better: could do elliptical arc but this suffices)
#         r = (r_start + r_end) / 2
#         arcs.append({
#             "center": Vector2.from_xy(mm(center_mm[0]), mm(center_mm[1])),
#             "radius": mm(r),
#             "start_angle": start_angle,
#             "sweep_angle": sweep_angle
#         })
#     return arcs

# if __name__ == '__main__':
#     kicad = KiCad()
#     board = kicad.get_board()

#     center_mm = (100, 100)
#     start_radius_mm = 1.0
#     spacing_mm = 0.5
#     turns = 8
#     arcs_per_turn = 8  # much fewer arcs than line segments
#     track_width_mm = 0.25
#     layer_front = BoardLayer.BL_F_SilkS
#     layer_back = BoardLayer.BL_B_Cu

#     # Front spiral (outward)
#     arcs_front = generate_spiral_arcs(center_mm, start_radius_mm, spacing_mm, turns, arcs_per_turn, outward=True)
#     for arc_params in arcs_front:
#         arc = BoardArc()
#         arc.layer = layer_front
#         arc.width = mm(track_width_mm)
#         arc.center = arc_params["center"]
#         arc.radius = arc_params["radius"]
#         arc.start_angle = arc_params["start_angle"]
#         arc.sweep_angle = arc_params["sweep_angle"]
#         board.create_items(arc)
#         print(arc_params)

#     # # Place via at outer end of front spiral
#     # # Calculate outermost radius point angle
#     # outer_angle_deg = turns * 360
#     # outer_radius_mm = start_radius_mm + spacing_mm * turns
#     # via_pos = Vector2.from_xy(
#     #     mm(center_mm[0] + outer_radius_mm * math.cos(math.radians(outer_angle_deg))),
#     #     mm(center_mm[1] + outer_radius_mm * math.sin(math.radians(outer_angle_deg)))
#     # )
#     # via = Via()
#     # via.position = via_pos
#     # via.diameter = mm(0.8)
#     # via.drill = mm(0.4)
#     # via.layers = [layer_front, layer_back]
#     # board.create_items(via)

#     # # Back spiral (inward)
#     # arcs_back = generate_spiral_arcs(center_mm, start_radius_mm, spacing_mm, turns, arcs_per_turn, outward=False)
#     # for arc_params in arcs_back:
#     #     arc = BoardArc()
#     #     arc.layer = layer_back
#     #     arc.width = mm(track_width_mm)
#     #     arc.center = arc_params["center"]
#     #     arc.radius = arc_params["radius"]
#     #     arc.start_angle = arc_params["start_angle"]
#     #     arc.sweep_angle = arc_params["sweep_angle"]
#     #     board.create_items(arc)

#     # print("✅ Dual-spiral (arcs) with via created.")

from kipy import KiCad
from kipy.geometry import Vector2
from kipy.board import BoardLayer
from kipy.board_types import BoardArc

def mm(x): return int(x * 1_000_000)

kicad = KiCad()
board = kicad.get_board()

arc = BoardArc()
arc.layer = BoardLayer.BL_Eco1_User
arc.width = mm(0.2)
arc.center = Vector2.from_xy(mm(100), mm(100))
arc.radius = mm(10)
arc.start_angle = 0
arc.sweep_angle = 90

board.create_items(arc)

print("✅ Arc added.")
