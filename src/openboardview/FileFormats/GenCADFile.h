#ifndef GENCADFILE_H
#define GENCADFILE_H

#include "BRDFileBase.h"

#include "build-generated/GenCADFileGrammar.h"

#include "mpc/mpc.h"

#include <map>
#include <string>
#include <vector>

enum ParseVarsCounterEnum {
#define X(CVAR, NAME) _ignore_me_##CVAR,
	X_MACRO_PARSE_VARS
	PARSE_VARS_COUNT
#undef X
};

class GenCADFile : public BRDFileBase {
  public:
	static bool verifyFormat(const std::vector<char> &buf);

	GenCADFile(const std::vector<char> &buf);

	enum Dimension {
		INCH,   // Inches.
		THOU,   // for thousandths of an inch.
		MM,     // Millimeters.
		MM100,  // for hundredths of a millimeter.
		USER,   // <p_integer>—number of units to the inch.
		USERCM, // <p_integer>—number of units to a centimeter.
		USERMM, // <p_integer>—number of units to a millimeter.
		INVALID,
	};

  private:
	enum Dimension m_dimension = INCH;
	int m_dimension_unit       = 0;
	bool parse_file(const std::vector<char> &buf);

	bool parse_dimension_units(mpc_ast_t *header_ast);
	bool parse_board_outline(mpc_ast_t *board_ast);
	std::vector<std::pair<BRDPoint, BRDPoint>> arc_to_segments_from_ast(mpc_ast_t *start, mpc_ast_t *stop, mpc_ast_t *center);
	bool parse_vias(mpc_ast_t *routes_ast);
	bool parse_route_vias(mpc_ast_t *route_ast);
	bool parse_components();

	bool
	parse_shape_pins_to_component(BRDPart *part, double rotation_in_degrees, bool mirror_x, bool mirror_y, bool flip, mpc_ast_t *shape_ast);

	void fill_signals_cache();
	const char *get_signal_name_for_component_pin(const char *component_name, mpc_ast_t *pin_ast);
	mpc_ast_t *get_shape_by_name(const char *name);
	char *get_nonquoted_or_quoted_string_child(mpc_ast_t *parent, const char *name);

	// returns true if content_holder is not null, and contains value exactly equal to passed non-null text
	bool has_text_content(mpc_ast_t *content_holder, const char *text);
	char *get_stringtoend_child(mpc_ast_t *parent, const char *name);
	mpc_ast_t *get_padstack_by_name(const char *padstack_name);
	mpc_ast_t *get_pad_by_name(const char *pad_name);
	double get_padstack_radius(mpc_ast_t *padstack_ast);
	BRDPinSide get_padstack_side(mpc_ast_t *padstack_ast);
	double get_pad_radius(mpc_ast_t *pad_ast);

	int board_unit_to_brd_coordinate(double brdUnit);
	bool x_y_ref_to_brd_point(mpc_ast_t *x_y_ref, BRDPoint *point);

	bool is_shape_smd(mpc_ast_t *shape_ast);
	bool is_padstack_drilled(mpc_ast_t *padstack_ast);

	mpc_ast_t *header_ast     = nullptr;
	mpc_ast_t *signals_ast    = nullptr;
	mpc_ast_t *devices_ast    = nullptr;
	mpc_ast_t *shapes_ast     = nullptr;
	mpc_ast_t *components_ast = nullptr;
	mpc_ast_t *pads_ast       = nullptr;
	mpc_ast_t *padstacks_ast  = nullptr;

	typedef std::tuple<std::string, std::string> ComponentPin;
	std::map<ComponentPin, std::string> m_signals_cache;
	int nc_counter = 0;

};

#endif // GENCADFILE_H
