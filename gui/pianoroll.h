#ifndef _PIANO_ROLL_H_
#define _PIANO_ROLL_H_

#include <agar/core.h>
#include <agar/gui.h>

class PianoRoll {
	public:
		PianoRoll(void* parent) {
			VG* vg = VG_New(0);

			VG_Point* pa = VG_PointNew(vg->root, VG_GetVector(  0,  0));
			VG_Point* pb = VG_PointNew(vg->root, VG_GetVector(100,  0));
			VG_Point* pc = VG_PointNew(vg->root, VG_GetVector(100, 10));
			VG_Point* pd = VG_PointNew(vg->root, VG_GetVector(  0, 10));

			VG_Polygon* poly = VG_PolygonNew(vg->root);
			VG_PolygonVertex(poly, pa);
			VG_PolygonVertex(poly, pb);
			VG_PolygonVertex(poly, pc);
			VG_PolygonVertex(poly, pd);
			VG_SetColorRGB(poly, 20, 128, 60);

			pa = VG_PointNew(vg->root, VG_GetVector(5200,  0));
			pb = VG_PointNew(vg->root, VG_GetVector(5300,  0));
			pc = VG_PointNew(vg->root, VG_GetVector(5300, 10));
			pd = VG_PointNew(vg->root, VG_GetVector(5200, 10));

			poly = VG_PolygonNew(vg->root);
			VG_PolygonVertex(poly, pa);
			VG_PolygonVertex(poly, pb);
			VG_PolygonVertex(poly, pc);
			VG_PolygonVertex(poly, pd);
			VG_SetColorRGB(poly, 20, 128, 60);

			_view = VG_ViewNew(parent, vg, VG_VIEW_HFILL | VG_VIEW_VFILL | VG_VIEW_GRID);
			_vg = vg;

			//Test: Try to create a constrained-move tool
			_toolOps.name = "Constrained move";
			_toolOps.desc = "Constrained move";
			_toolOps.icon = NULL;
			_toolOps.len = sizeof(VG_Tool);
			_toolOps.flags = VG_NOSNAP;
			_toolOps.init = NULL;
			_toolOps.destroy = NULL;
			_toolOps.edit = NULL;
			_toolOps.predraw = NULL;
			_toolOps.postdraw = NULL;
			_toolOps.selected = NULL;
			_toolOps.deselected = NULL;
			_toolOps.mousemotion = NULL;
			_toolOps.mousebuttondown = NULL;
			_toolOps.mousebuttonup = NULL;
			_toolOps.keydown = NULL;
			_toolOps.keyup = NULL;

			_tool = VG_ViewRegTool(_view, &_toolOps, NULL);
			VG_ViewSelectTool(_view, _tool, NULL);

			_grid = 0;
			VG_ViewSetGrid(_view, _grid, VG_GRID_LINES, 10, VG_GetColorRGB(50, 50, 50));
		}

		~PianoRoll() {
			VG_Destroy(_vg);
		}
	private:
		VG_View* _view;
		VG* _vg;

		int _grid;

		VG_ToolOps _toolOps;
		VG_Tool* _tool;
};

#endif

