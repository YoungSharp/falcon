/* $Id: glg.c,v 1.17 2007/07/21 13:14:36 jscott Exp $  
 *
 * Modified by Paul Filitchkin 05/02/2009
 *
 * A GTK+ widget that implements a modified XY line graph.
 * - Y points are plotted
 * - X point on all series is implied - by entry order
 * - X-scale rolls to display the most recent date (i.e. show last 40 number of points)
 * - Support unlimited number of data series
 * - Popup legend via mouse-button one
 * - Supports X Y & Page Titles
 *   
 *
 * (c) 2007, James Scott Jr
 *
 * Authors:
 *   James Scott Jr <skoona@users.sourceforge.net>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
 
/**
 * SECTION:glg
 * @short_description: A simple xy line graph widget written using only GTK/GDK.
 * @see_also: #GlgLineGraph
 * @stability: Stable
 * @include: glg.h
 *
 * <mediaobject>
 *   <imageobject>
 *     <imagedata fileref="glg.png"/> 
 *   </imageobject>
 * </mediaobject>
 *
 *    A Gtk2/GLib2 GUI application which demonstrates the use of GTK+ for
 * producing xy line graphs.  This widget once created allows you to add one or more
 * data series, then add values to those series for ploting.  The X point is assumed based
 * on arrival order.  However, the Y value or position is based one the current scale and
 * the y value itself.  If the charts x scale maximum is 40, or 40 points, the 41+ value is
 * appended to the 40th position after pos 0 is dropped - effectively rolling the x points
 * from right to left in the chart view.
 *
 * Packaged as a gtk widget for ease of use and easy inclusion into new or existing programs.
 * 
 * The GlgLinegraph widget has a gobject property for every control option except creating a 
 * new data series - glg_line_graph_series_add(), and adding a value to that series 
 * - glg_line_graph_series_add_value().
 * 
 * One signal is available 'point-selected' which outputs the Y value most likely under the 
 * mouse ptr.  For correlation purposes the position of the mouse and the position of the Y point
 * is given in case two or more points are returned.
 *
 * The scale or range of the chart is dependant on the Y-values.  For value greater than 1, the range/scale
 * should be set to whole numbers say 0 to 100.  For values less than 1, use a range/scale of 0 to 1 .
 *  
 * The following api's will create a version of this line graph:
 * <example>
 *  <title>Using a GlgLineGraph with gobject methods.</title>
 *  <programlisting>
 *  #include <glg.h>
 *  ...
 *  GlgLineGraph *glg = NULL;
 *  gint  i_series_0 = 0, i_series_1 = 0;
 *  ...
 *  glg = (GlgLineGraph *) g_object_new (GLG_TYPE_LINE_GRAPH, NULL); 
 *
 *  g_object_set (glg,  "range-tick-minor-x", 1,
 *   					"range-tick-major-x", 2,
 *   					"range-scale-minor-x", 0,
 *   					"range-scale-major-x", 40,
 *   					"range-tick-minor-y", 5,
 *   					"range-tick-major-y", 10,
 *   					"range-scale-minor-y", 0,
 *   					"range-scale-major-y", 100,
 *   					NULL);
 *
 *  g_object_set (glg, "chart-set-elements", GLG_TOOLTIP | 
 *						GLG_GRID_LABELS_X | GLG_GRID_LABELS_Y |
 *                	 	GLG_TITLE_T | GLG_TITLE_X | GLG_TITLE_Y |                                     
 *                	 	GLG_GRID_LINES | GLG_GRID_MINOR_X | GLG_GRID_MAJOR_X |
 *                    	GLG_GRID_MINOR_Y | GLG_GRID_MAJOR_Y, 
 *                    			      NULL); 
 *
 *  g_object_set (glg, "series-line-width", 3, NULL);
 *
 *  g_object_set (glg,  "graph-title-foreground",  "blue",
 *   					"graph-scale-foreground",  "red",
 *    					"graph-chart-background",  "light blue",
 *    					"graph-window-background", "white", 
 *    					NULL);
 *
 *  g_object_set (glg,  "text-title-main", "This Top Title Line ",        				
 *                      "text-title-yaxis", "This is the Y axis title line.",
 *         				"text-title-xaxis", "This is the X axis title line.",
 *   			        NULL);
 *
 *  gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(glg));
 *  gtk_widget_show_all (window);
 * 
 *
 *  i_series_0 = glg_line_graph_data_series_add (glg, "Volts", "red");
 *  i_series_1 = glg_line_graph_data_series_add (glg, "Battery", "blue");
 *
 *  glg_line_graph_data_series_add_value (glg, i_series_0, 66.0);
 *  glg_line_graph_data_series_add_value (glg, i_series_0, 73.0);
 *
 *  glg_line_graph_data_series_add_value (glg, i_series_1, 56.8);
 *  glg_line_graph_data_series_add_value (glg, i_series_1, 83.6);
 *  
 *
 *  glg_line_graph_redraw ( graph );
 *   	   
 *  </programlisting>
 * </example>
 * 
 * Or the following standard api method will also work.
 * <example>
 *  <title>Using a GlgLineGraph with standard apis.</title>
 *  <programlisting>
 *  #include <glg.h>
 *  ...
 * GlgLineGraph *glg = NULL;
 * gint  i_series_0 = 0, i_series_1 = 0;
 * ...
 * glg = (GlgLineGraph *) glg_line_graph_new();
 *
 * glg_line_graph_chart_set_ranges (glg, 1,2,0,40, 5,10,0,100);
 *
 * glg_line_graph_chart_set_elements (glg, GLG_TOOLTIP | 
 *				GLG_GRID_LABELS_X | GLG_GRID_LABELS_Y |
 *               	 	GLG_TITLE_T | GLG_TITLE_X | GLG_TITLE_Y |                                     
 *               	 	GLG_GRID_LINES | GLG_GRID_MINOR_X | GLG_GRID_MAJOR_X |
 *                   	GLG_GRID_MINOR_Y | GLG_GRID_MAJOR_Y 
 *                   			     ); 
 *
 * glg_line_graph_chart_set_color (graph, GLG_TITLE,  "blue");
 * glg_line_graph_chart_set_color (graph, GLG_SCALE,  "read");
 * glg_line_graph_chart_set_color (graph, GLG_CHART,  "light blue");
 * glg_line_graph_chart_set_color (graph, GLG_WINDOW, "white");	
 *
 * glg_line_graph_chart_set_text (glg, GLG_TITLE_T, "This Top Title Line " );	
 *        				
 * glg_line_graph_chart_set_text (glg, GLG_TITLE_Y, "This is the y label." ); 	
 *        					 
 * glg_line_graph_chart_set_text (glg, GLG_TITLE_X, "This is the x label" );
 * 
 * gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(glg));
 * gtk_widget_show_all (window);
 *
 * i_series_0 = glg_line_graph_data_series_add (glg, "Volts", "red");
 * i_series_1 = glg_line_graph_data_series_add (glg, "Battery", "blue");
 *
 * glg_line_graph_data_series_add_value (glg, i_series_0, 66.0);
 * glg_line_graph_data_series_add_value (glg, i_series_0, 73.0);
 *
 * glg_line_graph_data_series_add_value (glg, i_series_1, 56.8);
 * glg_line_graph_data_series_add_value (glg, i_series_1, 83.6);
 *
 *  glg_line_graph_redraw ( graph );
 * 	   
 *  </programlisting>
 * </example>
 *     
 *
 * A 'linegraph.c' demonstration proram is included to illustrate how to quickly use the widget.
 *
 * <emphasis>FEATURES</emphasis>
 * <itemizedlist>
 *  <listitem>
 *  Unlimited data series support.
 *  </listitem>
 *  <listitem>
 *  Accurate scaling across a wide range of X & Y scales.
 *  </listitem>
 *  <listitem>
 *  Using values ranges above or below 1.
 *  </listitem>
 *  <listitem>
 *  Rolling data points, if number of x points exceed x-scale. (left shift)
 *  </listitem>
 *  <listitem>
 *  Ability to change chart background color, window backgrounds colors, etc.
 *  </listitem>
 *  <listitem>
 *  Popup Tooltip, via mouse-button-1 click to enable/toggle. Tooltip overlays top graph title, when present.
 *  </listitem>
 *  <listitem>
 *  Data points are time stamped with current time when added.
 *  </listitem>
 *  <listitem>
 *  Some key debug messages to console with button-3 enable, then button-2 click/toggle.
 *  </listitem>
 *  <listitem>
 *  Auto Size to current window size; i.e. no-scrolling.
 *  </listitem>
 * </itemizedlist>
 * <example>
 *  <title>A sample program</title>
 * <programlisting>
 *  <link fileref="linegraph.c"/> 
 * </programlisting>
 * </example>
 * 
 */


#include <gtk/gtk.h>
#include <glib-object.h>
#include <math.h>
#include <time.h>

#include "glg.h"


#define GLG_MAX_STRING  256      /* Size of a text string */
#define GLG_MAX_BUFFER  512      /* Size of a text buffer or local string */

#define GLG_USER_MODEL_X 540
#define GLG_USER_MODEL_Y 300 

#define BORDER_2PAD 	2
#define BORDER_4PAD 	4


/*
 * Individual Data Series for Plotting
*/
typedef struct _GLG_SERIES {
    gint        cb_id;
    gint        i_series_id;    /* is this series number 1 2 or 3, ZERO based */
    gint        i_point_count;  /* 1 based */
    gint        i_max_points;   /* 1 based */
    gchar       ch_legend_text[GLG_MAX_STRING];
    gchar       ch_legend_color[GLG_MAX_STRING];
    GdkColor    legend_color;
    gdouble     d_max_value;
    gdouble     d_min_value;
    gdouble    *lg_point_dvalue;    /* array of doubles y values zero based, x = index */
    GdkPoint   *point_pos;      /* last gdk position each point - recalc on evey draw */
} GLG_SERIES, *PGLG_SERIES;

/*
 * Chart dimensions for drawing chart box
*/
typedef struct _GLG_RANGES {
    gint        cb_id;
    gint        i_inc_minor_scale_by;   /* minor increments */
    gint        i_inc_major_scale_by;   /* major increments */
    gint        i_min_scale;    /* minimum scale value - ex:   0 */
    gint        i_max_scale;    /* maximum scale value - ex: 100 */
    gint        i_num_minor;    /* number of minor points */
    gint        i_num_major;    /* number of major points */
    gint        i_minor_inc;    /* pixels per minor increment */
    gint        i_major_inc;    /* pixels per major increment */
} GLG_RANGE , *PGLG_RANGE;


/* 
 * widget private data structure 
*/
typedef struct _GlgLineGraphPrivate
{
    gint        cb_id;			  /* structure id */	
    GLGElementID lgflags;         /* things to be drawn */
    GdkGC      *window_gc;
    GdkGC      *box_gc;
    GdkGC      *scale_gc;
    GdkGC      *title_gc;
    GdkGC      *series_gc;
    gint       series_line_width;        /* drawn line width for data series -- default: 3 */

    /* data points and tooltip info */
    gint        i_points_available;
    gint        i_num_series;   /* 1 based */
    GList      *lg_series;      /* double-linked list of data series PGLG_SERIES */
    GList      *lg_series_time; /* time_t of each sample */

    /* actual size of graph area */
    GdkRectangle page_box;      /* entire window size */    
    gint        width_scale;
    gint        height_scale;
    /* buffer around all sides */
    gint        x_border;
    gint        y_border;
    gint		xfactor;               /* default pixel size of one 'M' */
    gint		yfactor;			   /* default pixel size of one 'M' */    
    /* current mouse position */
    gboolean    b_mouse_onoff;
    GdkPoint    mouse_pos;
    GdkModifierType mouse_state;
    /* top/left or baseline of labels and titles */
    gchar       ch_color_window_bg[GLG_MAX_STRING];
    gchar       ch_color_chart_bg[GLG_MAX_STRING];
    gchar       ch_color_title_fg[GLG_MAX_BUFFER];
    GdkRectangle x_label_box;
    GdkRectangle y_label_box;
    GdkRectangle page_title_box;

    gchar      *x_label_text;
    gchar      *y_label_text;
    gchar      *page_title_text;
    /* position and area of main graph plot area */
    GdkRectangle plot_box;
    gchar       ch_color_scale_fg[GLG_MAX_STRING];
    GLG_RANGE    x_range;
    GLG_RANGE    y_range;
} GlgLineGraphPrivate;

/* 
 * Internal working structure ids 
*/
typedef enum _GLG_Control_Block_id {
    GLG_NO_ID,
    GLG_SERIES_ID,
    GLG_RANGE_ID,
    GLG_GRAPH_ID,
    GLG_PRIVATE_ID,    
    GLG_NUM_ID
} GLGDataID;

/* 
 * Internal working property ids 
*/
enum _GLG_PROPERTY_ID {
  PROP_0,
  PROP_GRAPH_DRAWING_TYPE,
  PROP_GRAPH_TITLE,
  PROP_GRAPH_TITLE_X,
  PROP_GRAPH_TITLE_Y,
  PROP_GRAPH_LINE_WIDTH,
  PROP_GRAPH_ELEMENTS,
  PROP_GRAPH_TITLE_COLOR,      
  PROP_GRAPH_SCALE_COLOR,      
  PROP_GRAPH_CHART_COLOR,      
  PROP_GRAPH_WINDOW_COLOR,
  PROP_TICK_MINOR_X,
  PROP_TICK_MAJOR_X,
  PROP_SCALE_MINOR_X,
  PROP_SCALE_MAJOR_X,
  PROP_TICK_MINOR_Y,
  PROP_TICK_MAJOR_Y,
  PROP_SCALE_MINOR_Y,
  PROP_SCALE_MAJOR_Y
} GLG_PROPERTY_ID;


G_DEFINE_TYPE (GlgLineGraph, glg_line_graph, GTK_TYPE_DRAWING_AREA);

#define GLG_LINE_GRAPH_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GLG_TYPE_LINE_GRAPH, GlgLineGraphPrivate))

/*
 * Private routines for graph widget internal functions
*/
static void 	glg_line_graph_class_init (GlgLineGraphClass *klass);
static void 	glg_line_graph_init (GlgLineGraph *graph);
static void 	glg_line_graph_destroy (GtkObject *object);
static void 	glg_line_graph_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void 	glg_line_graph_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);

static gboolean glg_line_graph_configure_event (GtkWidget *widget, GdkEventConfigure *event);
static gboolean glg_line_graph_expose (GtkWidget *graph, GdkEventExpose *event);
static gboolean glg_line_graph_button_press_event (GtkWidget * widget, GdkEventButton * ev);

static void 	glg_line_graph_draw (GtkWidget *graph);

static void 	glg_line_graph_draw_x_grid_labels (GlgLineGraph *graph);
static void 	glg_line_graph_draw_y_grid_labels (GlgLineGraph *graph);
static gint 	glg_line_graph_draw_text_horizontal (GlgLineGraph *graph, gchar * pch_text, GdkRectangle * rect, gboolean redraw_control);
static gint 	glg_line_graph_draw_text_vertical (GlgLineGraph *graph, gchar * pch_text, GdkRectangle * rect, gboolean redraw_control);
static gint 	glg_line_graph_draw_grid_lines (GlgLineGraph *graph);

static gint 	glg_line_graph_data_series_draw (GlgLineGraph *graph, PGLG_SERIES psd);
static gint 	glg_line_graph_data_series_draw_all (GlgLineGraph *graph, gboolean redraw_control);
static gint 	glg_line_graph_data_series_remove_all (GlgLineGraph *graph);

static void _glg_marshal_VOID__DOUBLE_DOUBLE_DOUBLE_DOUBLE (GClosure     *closure,
                                                      GValue       *return_value,
                                                      guint         n_param_values,
                                                      const GValue *param_values,
                                                      gpointer      invocation_hint,
                                                      gpointer      marshal_data);


/*
 * Global diagnostics varible
*/
static gboolean glg_flag_debug = FALSE;

enum
{
	POINT_SELECTED_SIGNAL,
	LAST_SIGNAL
};

static guint glg_line_graph_signals[LAST_SIGNAL] = { 0 };

/*
 * Function definitions
*/
static void glg_line_graph_class_init (GlgLineGraphClass *klass)
{
	GObjectClass 	*obj_class 		 = G_OBJECT_CLASS (klass);
	GtkWidgetClass  *widget_class 	 = GTK_WIDGET_CLASS (klass);
    GtkObjectClass  *gtkobject_class = GTK_OBJECT_CLASS (klass);
    gint			elements = GLG_GRID_LINES;      

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_class_init()");
	}

    /* GObject signal overrides */
    obj_class->set_property = glg_line_graph_set_property;
    obj_class->get_property = glg_line_graph_get_property;

	/* GtkObject signal overrides */
	gtkobject_class->destroy    = glg_line_graph_destroy;

	/* GtkWidget signals overrides */
	widget_class->configure_event 	  = glg_line_graph_configure_event;
	widget_class->expose_event 		  = glg_line_graph_expose;   
  	widget_class->button_press_event  = glg_line_graph_button_press_event;

	/**
	 * GlgLineGraph::point-selected:
	 * @widget: the line graph widget that received the signal
	 * @x_value: x value on the chart
	 * @y_value: y value on the chart
	 * @point_y_pos: pixel position of y value on chart
	 * @mouse_y_pos: pixel position of the mouse ptr on chart
	 *
	 * The ::point-selected signal is emitted after the toggle-on mouse1 click, and sends
	 * values closest to the mouse pointer.  
	 */
	glg_line_graph_signals[POINT_SELECTED_SIGNAL] = g_signal_new (
			"point-selected",
			G_OBJECT_CLASS_TYPE (obj_class),
			G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET (GlgLineGraphClass, point_selected),
			NULL, NULL,
			_glg_marshal_VOID__DOUBLE_DOUBLE_DOUBLE_DOUBLE,
			G_TYPE_NONE, 4,
			G_TYPE_DOUBLE,
			G_TYPE_DOUBLE,
			G_TYPE_DOUBLE,
			G_TYPE_DOUBLE);

	g_type_class_add_private (obj_class, sizeof (GlgLineGraphPrivate));

  g_object_class_install_property (obj_class,
                                   PROP_GRAPH_TITLE,
                                   g_param_spec_string ("text-title-main",
                                                        "Graph Top Title",
                                                        "Title at top of graph on the X axis",
                                                        "<big><b>Top Title</b></big>",
                                                        G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
                                   PROP_GRAPH_TITLE_X,
                                   g_param_spec_string ("text-title-xaxis",
                                                        "Graph x axis title",
                                                        "Title at bottom of graph on the X axis",
                                                        "<i>X Axis Title</i>",
                                                        G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
                                   PROP_GRAPH_TITLE_Y,
                                   g_param_spec_string ("text-title-yaxis",
                                                        "Graph y axis title",
                                                        "Title on left of graph on the Y axis",
                                                        "Y Axis Title",
                                                        G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
                                   PROP_GRAPH_LINE_WIDTH,
                                   g_param_spec_int  ("series-line-width",
                                                      "Series line width",
                                                      "Width of line drawn for data series",
                                                       1,10,3,             
                                                       G_PARAM_READWRITE));  
  g_object_class_install_property (obj_class,
                                   PROP_GRAPH_ELEMENTS,
                                   g_param_spec_int  ("chart-set-elements",
                                                      "Show Chart Elements",
                                                      "Enable showing these elements of the chart body",
                                                       0, GLG_RESERVED_ON, elements,             
                                                       G_PARAM_WRITABLE));  
        
  g_object_class_install_property (obj_class,
                                   PROP_GRAPH_TITLE_COLOR,
                                   g_param_spec_string ("graph-title-foreground",
                                                        "Color name",
                                                        "Main title foreground color",
                                                        "blue",
                                                        G_PARAM_WRITABLE));          
  g_object_class_install_property (obj_class,
                                   PROP_GRAPH_SCALE_COLOR,
                                   g_param_spec_string ("graph-scale-foreground",
                                                        "Color name",
                                                        "X and Y chart scale foreground font color",
                                                        "black",
                                                        G_PARAM_WRITABLE));          
  g_object_class_install_property (obj_class,
                                   PROP_GRAPH_CHART_COLOR,
                                   g_param_spec_string ("graph-chart-background",
                                                        "Color name",
                                                        "Chart inside fill color",
                                                        "light blue",
                                                        G_PARAM_WRITABLE));  
    g_object_class_install_property (obj_class,
                                   PROP_GRAPH_WINDOW_COLOR,
                                   g_param_spec_string ("graph-window-background",
                                                        "Color name",
                                                        "Window background fill color",
                                                        "white",
                                                        G_PARAM_WRITABLE));  
/* *** */
  g_object_class_install_property (obj_class,
  									PROP_TICK_MINOR_X,
                                   g_param_spec_int  ("range-tick-minor-x",
                                                      "x minor tick increment",
                                                      "x minor ticks on scale",
                                                       1, 1000000000, 5,             
                                                       G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
  									PROP_TICK_MAJOR_X,
                                   g_param_spec_int  ("range-tick-major-x",
                                                      "x major tick increment",
                                                      "x major ticks on scale",
                                                       1, 1000000000, 10,             
                                                       G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
  									PROP_SCALE_MINOR_X,
                                   g_param_spec_int  ("range-scale-minor-x",
                                                      "x minor scale range",
                                                      "x minor scale range",
                                                       0, 1000000000, 0,             
                                                       G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
								   PROP_SCALE_MAJOR_X,
                                   g_param_spec_int  ("range-scale-major-x",
                                                      "x major scale range",
                                                      "x major scale range",
                                                       1, 1000000000, 100,             
                                                       G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
  									PROP_TICK_MINOR_Y,
                                   g_param_spec_int  ("range-tick-minor-y",
                                                      "Y minor tick increment",
                                                      "Y minor ticks on scale",
                                                       1, 1000000000, 5,             
                                                       G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
  									PROP_TICK_MAJOR_Y,
                                   g_param_spec_int  ("range-tick-major-y",
                                                      "Y major tick increment",
                                                      "Y major ticks on scale",
                                                       1, 1000000000, 10,             
                                                       G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
  									PROP_SCALE_MINOR_Y,
                                   g_param_spec_int  ("range-scale-minor-y",
                                                      "Y minor scale range",
                                                      "Y minor scale range",
                                                       0, 1000000000, 0,             
                                                       G_PARAM_WRITABLE));  
  g_object_class_install_property (obj_class,
								   PROP_SCALE_MAJOR_Y,
                                   g_param_spec_int  ("range-scale-major-y",
                                                      "Y major scale range",
                                                      "Y major scale range",
                                                       1, 1000000000, 100,             
                                                       G_PARAM_WRITABLE));  
		
	return;
}

static void glg_line_graph_init (GlgLineGraph *graph)
{
	GdkWindow  *gdk_window = NULL;
	GlgLineGraphPrivate *priv = NULL;
		
	if (glg_flag_debug) {
		g_debug ("glg_line_graph_init()");
	}
	g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
	
	gtk_widget_add_events (GTK_WIDGET (graph), GDK_BUTTON_PRESS_MASK | 
											   GDK_BUTTON_RELEASE_MASK |
											   GDK_POINTER_MOTION_MASK |											   
											   GDK_POINTER_MOTION_HINT_MASK);
	priv->cb_id = GLG_PRIVATE_ID;
	priv->b_mouse_onoff = FALSE;
	if (priv->series_line_width < 1) { 
		priv->series_line_width = 2;
	}

	gdk_window = gdk_get_default_root_window ();
  	priv->series_gc = gdk_gc_new (gdk_window);
  	priv->window_gc = gdk_gc_new (gdk_window);
  	priv->box_gc    = gdk_gc_new (gdk_window);
  	priv->scale_gc  = gdk_gc_new (gdk_window);
  	priv->title_gc  = gdk_gc_new (gdk_window);
	    
	return;
}


static gboolean glg_line_graph_configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
	GlgLineGraphPrivate *priv;
    GlgLineGraph   *graph = GLG_LINE_GRAPH(widget);
    gint        xfactor = 0, 
    			yfactor = 0; 
    PangoLayout *layout;
  	
	if (glg_flag_debug) {
		g_debug ("===> glg_line_graph_configure_event()");
	}
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);
	
	g_return_val_if_fail ( event->type == GDK_CONFIGURE, FALSE);
	
	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
	g_return_val_if_fail ( priv != NULL, FALSE);	

    if (glg_flag_debug) { 
		g_debug ("===> glg_line_graph_configure_event(new width=%d, height=%d)", event->width, event->height);
    } 

	/*
	 * Capture new size */
    priv->page_box.width = event->width;
	priv->page_box.height = event->height;		

  	/* 
  	 * Create a PangoLayout, get the spacing of one large char to use as a standard */
  	layout = gtk_widget_create_pango_layout (widget, "M");
		pango_layout_set_markup (layout, "<b>M</b>", -1);
		pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);
        pango_layout_get_pixel_size (layout, &xfactor, &yfactor);
		priv->xfactor = xfactor = ((xfactor+6)/10) * 10;
		priv->yfactor = yfactor = ((yfactor+6)/10) * 10;
	    if (glg_flag_debug) {
			g_debug ("cfg:factors:pango_layout_get_pixel_size(width=%d, height=%d)", xfactor, yfactor);
	    }
  	g_object_unref (layout);

    /*
     * Setup chart rectangles */	
	priv->x_border = xfactor / 2;		 /* def 16/2=8 edge pad */
	priv->y_border = yfactor / 5;	         /* def 20/5=4 edge pad */
    
    if (priv->lgflags & GLG_TITLE_T) {        
    	priv->page_title_box.x = xfactor * 6;       /* define top-left corner of textbox */
    	priv->page_title_box.y = priv->y_border;
    	priv->page_title_box.width  = priv->page_box.width - priv->page_title_box.x - priv->x_border;
		priv->page_title_box.height = yfactor * 2;
    }
    if (priv->lgflags & GLG_TITLE_X) {    
        priv->x_label_box.x = xfactor * 6;          /* define top-left corner of textbox */
        priv->x_label_box.y = priv->page_box.height - (yfactor * 1) - priv->y_border;
    	priv->x_label_box.width  = priv->page_box.width - priv->x_label_box.x - priv->x_border;
    	priv->x_label_box.height = (yfactor * 1) + priv->y_border;
    }
    if (priv->lgflags & GLG_TITLE_Y) {             /* define top-left corner of textbox */
    	priv->y_label_box.x = BORDER_2PAD;
    	priv->y_label_box.y = (yfactor * 3);
    	priv->y_label_box.width  = xfactor * 2;
    	priv->y_label_box.height = priv->y_label_box.y - (yfactor * 3);
    }


    /*
     * This is for the main chart area or plot box 
     * -- this calc is for the maximum available area 
     */
    priv->plot_box.x = priv->y_label_box.width + priv->y_label_box.x + (xfactor * 2) + priv->x_border;
    priv->plot_box.y = priv->page_title_box.height + priv->page_title_box.y + priv->y_border;
    priv->plot_box.width  = (gint) priv->page_box.width - priv->plot_box.x - xfactor;
    priv->plot_box.height = (gint) priv->page_box.height - priv->plot_box.y  - priv->x_label_box.height - yfactor;
   
    if (glg_flag_debug) {
    	g_debug ("cfg:Max.Avail: plot_box.width=%d, plot_box.height=%d", 
    			  priv->plot_box.width, priv->plot_box.height);
    }

    /* 
     * reposition the box according to scale-able increments 
     * -- this calc is to align to scaling requirements 
     */
    xfactor = priv->plot_box.width;
    yfactor = priv->plot_box.height;
    priv->plot_box.width  = ( (gint)(priv->plot_box.width  / priv->x_range.i_num_minor) * priv->x_range.i_num_minor);
    priv->plot_box.height = ( (gint)(priv->plot_box.height / priv->y_range.i_num_minor) * priv->y_range.i_num_minor);

	/*
	 * Distribute the difference toward the bottom right
	 */
    xfactor -= priv->plot_box.width;
    yfactor -= priv->plot_box.height;
    priv->plot_box.x += (gint)(xfactor * 0.80);
    priv->plot_box.y += yfactor;    
    priv->page_title_box.x = priv->plot_box.x;
	priv->page_title_box.width = priv->plot_box.width;
    priv->x_label_box.x = priv->plot_box.x;
	priv->x_label_box.width = priv->plot_box.width;
	priv->y_label_box.y = priv->plot_box.y;
	priv->y_label_box.height = priv->plot_box.height;	
	
    /*
     * Determine the pixel increment of the grid lines 
     */
    priv->y_range.i_minor_inc = priv->plot_box.height / priv->y_range.i_num_minor;
    priv->y_range.i_major_inc = priv->plot_box.height / priv->y_range.i_num_major;
    priv->x_range.i_minor_inc = priv->plot_box.width / priv->x_range.i_num_minor;
    priv->x_range.i_major_inc = priv->plot_box.width / priv->x_range.i_num_major;

    if (glg_flag_debug) {
    	g_debug ("cfg:Chart:Incs: x_minor=%d, x_major=%d, y_minor=%d, y_major=%d, plot_box.width=%d, plot_box.height=%d",
	      priv->x_range.i_minor_inc,
	      priv->x_range.i_major_inc,
	      priv->y_range.i_minor_inc,
	      priv->y_range.i_major_inc,
	      priv->plot_box.width,
	      priv->plot_box.height);

    	g_debug ("cfg:Chart:Nums: x_num_minor=%d, x_num_major=%d, y_num_minor=%d, y_num_major=%d",
	      priv->x_range.i_num_minor,
	      priv->x_range.i_num_major,
	      priv->y_range.i_num_minor,
	      priv->y_range.i_num_major);
    }
    
    
    return FALSE;
}


/*
 * This routine is the master paint routine for the line graph
 * it sets up the deminsions of the space and call DRAW to get the graph done
*/
static gboolean glg_line_graph_expose (GtkWidget *graph, GdkEventExpose *event)
{
	GlgLineGraphPrivate *priv;
    gint        xfactor = 0, yfactor = 0, chart_set_ranges = 0;
    GtkWidget   *widget = graph;

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_expose()");
	}
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);
	
	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
	g_return_val_if_fail ( priv != NULL, FALSE);	

    /* 
     * trip to ensure chart ranges are already set 
     */ 
    xfactor = MAX (priv->x_range.i_num_minor, priv->x_range.i_num_major);
    yfactor = MAX (priv->y_range.i_num_minor, priv->y_range.i_num_major);

	chart_set_ranges = MIN (xfactor, yfactor);   
	g_return_val_if_fail (chart_set_ranges != 0, FALSE);	
	
    /* --- Erase the old stuff --- */
    gdk_draw_rectangle (widget->window, priv->window_gc,
                        TRUE, 0, 0, 
                        priv->page_box.width, 
                        priv->page_box.height);

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_expose(width=%d, height=%d)",priv->page_box.width, priv->page_box.height);
	}

	/* Draw the actual Chart */
    glg_line_graph_draw (graph);

	return FALSE;
}


/**
 * glg_line_graph_redraw:
 * @graph: pointer to a #GlgLineGraph widget
 * 
 * Updates the current graph showing new changes.
*/
extern void glg_line_graph_redraw (GlgLineGraph *graph)
{
	GtkWidget *widget;
	GdkRegion *region;
	
	if (glg_flag_debug) {
		g_debug ("glg_line_graph_redraw()");
	}
	g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));
		
	widget = GTK_WIDGET (graph);

	if (!widget->window) return;

	region = gdk_drawable_get_clip_region (widget->window);
	/* redraw the window completely by exposing it */
	gdk_window_invalidate_region (widget->window, region, TRUE);
/*	gdk_window_process_updates (widget->window, TRUE); */

	gdk_region_destroy (region);
}

/**
 * glg_line_graph_new:
 *
 * Creates a new line graph widget
 *
 * Returns:  a pointer to a #GlgLineGraph widget 
*/
extern GtkWidget * glg_line_graph_new (void)
{
	if (glg_flag_debug) {
		g_debug ("glg_line_graph_new()");
	}
	return g_object_new (GLG_TYPE_LINE_GRAPH, NULL);
}

/**
 * glg_line_graph_chart_set_ranges:
 * @graph: pointer to a #GlgLineGraph widget
 * @x_tick_minor: number of minor divisions for x scale
 * @x_tick_major: number of major divisions for x scale
 * @x_scale_min:  minimum x scale value or starting point
 * @x_scale_max:  maximum x scale value or ending point
 * @y_tick_minor: number of minor divisions for y scale
 * @y_tick_major: number of major divisions for y scale
 * @y_scale_min:  minimum y scale value or starting point
 * @y_scale_max:  maximum y scale value or ending point
 * 
 * Sets the X and Y ticks and scales for the graph grid area.
 */
extern void glg_line_graph_chart_set_ranges (GlgLineGraph *graph,
                                 gint x_tick_minor, gint x_tick_major,
                                 gint x_scale_min,  gint x_scale_max,
                                 gint y_tick_minor, gint y_tick_major, 
                                 gint y_scale_min,  gint y_scale_max)
{
	GlgLineGraphPrivate *priv;
	gint xfactor = 0, yfactor = 0, chart_set_ranges = 0;

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_chart_set_ranges()");
	}
	g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));
	
	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
	
	xfactor = MIN( x_tick_minor, x_tick_major );
	yfactor = MIN( y_tick_minor, y_tick_major );
	
	chart_set_ranges = MIN( xfactor, yfactor);    /* test for contextually vaild input */			
	g_return_if_fail ( chart_set_ranges != 0);

    if (priv->x_range.i_max_scale) {
	    g_message ("Set Ranges Failed: Cannot set ranges more than once, range already set!");
	    return;
	}	  

    priv->x_range.i_inc_minor_scale_by = x_tick_minor;  /* minimum scale value - ex:   1 */
    priv->x_range.i_inc_major_scale_by = x_tick_major;  /* minimum scale value - ex:   1 */

    priv->x_range.i_min_scale = x_scale_min;   /* minimum scale value - ex:   0 */
    priv->x_range.i_max_scale = x_scale_max;   /* maximum scale value - ex: 100 */
    priv->x_range.i_num_minor = x_scale_max / x_tick_minor;   /* number of minor points */
    priv->x_range.i_num_major = x_scale_max / x_tick_major;   /* number of major points */

    priv->y_range.i_inc_minor_scale_by = y_tick_minor;  /* minimum scale value - ex:   1 */
    priv->y_range.i_inc_major_scale_by = y_tick_major;  /* minimum scale value - ex:   0 */

    priv->y_range.i_min_scale = y_scale_min;   /* minimum scale value - ex:   0 */
    priv->y_range.i_max_scale = y_scale_max;   /* maximum scale value - ex: 100 */
    priv->y_range.i_num_minor = y_scale_max / y_tick_minor;   /* number of minor points */
    priv->y_range.i_num_major = y_scale_max / y_tick_major;   /* number of major points */

    return;
}

/**
 * glg_line_graph_chart_set_color:
 * @graph: pointer to a #GlgLineGraph widget
 * @element: #GLGElementID of the Window Element 
 * @pch_color: gchar* string with name of color.
 * 
 * Copy (without freeing) pch_color into place to be used as the graph element color. 
 * #GLG_SCALE  - x/y interger legends color {default black}
 * #GLG_TITLE  - Main graph title. {default light blue}
 * #GLG_WINDOW - Graph window background color, and grid foreground. {default white}
 * #GLG_CHART  - Graph plot area background. {default light blue}
 *
 * Returns: gboolean TRUE if copied, FALSE if copy failed.
 */
extern gboolean glg_line_graph_chart_set_color (GlgLineGraph *graph, GLGElementID element, const gchar * pch_color)
{
	GlgLineGraphPrivate *priv;
    GdkColor   color;
    gboolean   rc = TRUE;


	if (glg_flag_debug) {
		g_debug ("glg_line_graph_chart_set_color()");
	}
	g_return_val_if_fail (GLG_IS_LINE_GRAPH(graph), FALSE);
    g_return_val_if_fail (pch_color != NULL, FALSE);        
	
	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);


    switch ( element ) {
        case GLG_SCALE:
             g_snprintf (priv->ch_color_scale_fg, sizeof (priv->ch_color_scale_fg), 
                         "%s", pch_color);        
             gdk_color_parse (priv->ch_color_scale_fg, &color);
             gdk_gc_set_rgb_fg_color (priv->scale_gc, &color);
             break;        
        case GLG_TITLE:
             g_snprintf (priv->ch_color_title_fg, sizeof (priv->ch_color_title_fg), 
                         "%s", pch_color);        
             gdk_color_parse (priv->ch_color_title_fg, &color);
             gdk_gc_set_rgb_fg_color (priv->title_gc, &color);
             break;        
        case GLG_WINDOW:
             g_snprintf (priv->ch_color_window_bg, sizeof (priv->ch_color_window_bg),
                         "%s", pch_color);
             gdk_color_parse (priv->ch_color_window_bg, &color);
             gdk_gc_set_rgb_fg_color (priv->window_gc, &color);
             break;
        case GLG_CHART:
             g_snprintf (priv->ch_color_chart_bg, sizeof (priv->ch_color_chart_bg), 
                         "%s", pch_color);        
             gdk_color_parse (priv->ch_color_chart_bg, &color);
             gdk_gc_set_rgb_fg_color (priv->box_gc, &color);
             break;        
        default:
             g_message ("glg_line_graph_chart_set_color(): Invalid Element ID");
             rc = FALSE;
    }
    
   return rc;
}

/**
 * glg_line_graph_chart_set_text:
 * @graph: pointer to a #GlgLineGraph widget
 * @element: #GLGElementID of the Title 
 * @pch_text: gchar* string to set in title.
 * 
 * Copy (without freeing) pch_text into place to be used as the graph title. PangoMarkup is
 * also supported.
 *  
 * #GLG_TITLE_X - Bottom x axis title
 * #GLG_TITLE_Y - Left vertical y axis title
 * #GLG_TITLE_T - Top and considered main title on x axis
 * 
 * Returns: gboolean TRUE if copied, FALSE if copy failed.
 */
extern gboolean glg_line_graph_chart_set_text (GlgLineGraph *graph, GLGElementID element, const gchar *pch_text)
{
	GlgLineGraphPrivate *priv;
    gchar 	   *pch = NULL;
    gboolean   rc = TRUE;

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_chart_set_text()");
	}
	g_return_val_if_fail (GLG_IS_LINE_GRAPH(graph), FALSE);
    g_return_val_if_fail (pch_text != NULL, FALSE);        
	
	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);

    pch = g_strdup (pch_text);
    
    switch ( element ) {
        case GLG_TITLE_X:
             if (priv->x_label_text != NULL) {
                 g_free (priv->x_label_text);
             }
             priv->x_label_text = pch;
             break;        
        case GLG_TITLE_Y:
             if (priv->y_label_text != NULL) {
                 g_free (priv->y_label_text);
             }
             priv->y_label_text = pch;
             break;        
        case GLG_TITLE_T:
             if (priv->page_title_text != NULL) {
                 g_free (priv->page_title_text);
             }
             priv->page_title_text = pch;
             break;
        default:
             g_message ("glg_line_graph_chart_set_text(): Invalid Element ID");
             g_free ( pch );             
             rc = FALSE;
    }
    
  return rc;
}

static void glg_line_graph_draw (GtkWidget *graph)
{
	GlgLineGraphPrivate *priv;
	GLGElementID element = 0;

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_draw()");
	}
	g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
	
    /* 
     * draw plot area
     */
    gdk_draw_rectangle (graph->window,
                        priv->box_gc,
                        TRUE,
                        priv->plot_box.x,
                        priv->plot_box.y, priv->plot_box.width, priv->plot_box.height);

    gdk_gc_set_line_attributes (graph->style->black_gc,
                                2, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);
    gdk_draw_rectangle (graph->window,
                        graph->style->black_gc,
                        FALSE,
                        priv->plot_box.x,
                        priv->plot_box.y, priv->plot_box.width, priv->plot_box.height);

    if (glg_flag_debug) {
        g_debug
            ("Graph.Surface: Width=%d, Height=%d, Plot Area x=%d y=%d width=%d, height=%d",
             priv->page_box.width, priv->page_box.height,
             priv->plot_box.x, priv->plot_box.y, priv->plot_box.width, priv->plot_box.height);
    }
	

    /*
     * draw titles 
     */
    element = priv->lgflags;
    if ( element & GLG_TITLE_T) {
         glg_line_graph_draw_text_horizontal (GLG_LINE_GRAPH(graph), priv->page_title_text, &priv->page_title_box, FALSE);
    }
    if ( element & GLG_TITLE_X) {    
         glg_line_graph_draw_text_horizontal (GLG_LINE_GRAPH(graph), priv->x_label_text, &priv->x_label_box, FALSE);
    }
    if ( element & GLG_TITLE_Y) {
         glg_line_graph_draw_text_vertical (GLG_LINE_GRAPH(graph), priv->y_label_text, &priv->y_label_box, FALSE); 
    }   
    if ( ( element & GLG_GRID_LINES ) |
         ( element & GLG_GRID_MINOR_X ) |
         ( element & GLG_GRID_MAJOR_X ) |
         ( element & GLG_GRID_MINOR_Y ) |
         ( element & GLG_GRID_MAJOR_Y) ) { 
         glg_line_graph_draw_grid_lines (GLG_LINE_GRAPH(graph));
    }
    if ( element & GLG_GRID_LABELS_X) {
         glg_line_graph_draw_x_grid_labels (GLG_LINE_GRAPH(graph)); 
    }
    if ( element & GLG_GRID_LABELS_Y) {
         glg_line_graph_draw_y_grid_labels (GLG_LINE_GRAPH(graph)); 
    }

    glg_line_graph_data_series_draw_all (GLG_LINE_GRAPH(graph), FALSE); 
	
	return;	
}


/*
 * Draws a label text on the X axis
 * sets the width, height values of the input rectangle to the size of textbox
 * returns the width of the text area, or -1 on error
*/
static gint glg_line_graph_draw_text_horizontal (GlgLineGraph *graph, gchar * pch_text, GdkRectangle * rect, gboolean redraw_control)
{
	GlgLineGraphPrivate *priv;
    PangoLayout *layout = NULL;
    gint        x_pos = 0, y_pos = 0, width = 0, height = 0;;

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_draw_text_horizontal()");
	}
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);
    
    if (pch_text == NULL ) { return -1; }
    
    g_return_val_if_fail (rect != NULL, -1);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);

    layout = gtk_widget_create_pango_layout (GTK_WIDGET(graph), pch_text);
	    pango_layout_set_markup (layout, pch_text, -1);
	    pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);

	    pango_layout_get_pixel_size (layout, &width, &height);
	    if (width > rect->width ) {
	 	    x_pos = (priv->page_box.width - width) / 2;
	    } else {
	    	x_pos = rect->x + ((rect->width - width) /2);
	    }
	    if ( height > rect->height) {
	 	    y_pos = rect->y - ((height - rect->height) / 2);;  
	    } else {
	 	    y_pos = rect->y + (rect->height - height);
	    }

	if (glg_flag_debug) {
		g_debug ("Horiz.TextBox: x_pos=%d,  x=%d, y=%d, cx=%d, cy=%d",
				  x_pos, rect->x, rect->y, rect->width, rect->height);
	}


    gdk_draw_layout (GTK_WIDGET(graph)->window, priv->title_gc, x_pos, y_pos, layout);

    g_object_unref (layout);

    return rect->width;
}

/*
 * Draws a label text on the Y axis
 * sets the width, height values of the input rectangle to the size of textbox
 * returns the height of the text area, or -1 on error
*/
static gint glg_line_graph_draw_text_vertical (GlgLineGraph *graph, gchar * pch_text,
                                         GdkRectangle * rect, gboolean redraw_control)
{
	GlgLineGraphPrivate *priv;
    PangoRenderer *renderer = NULL;
    PangoMatrix matrix = PANGO_MATRIX_INIT;
    PangoContext *context = NULL;
    PangoLayout *layout = NULL;
    gint        y_pos = 0;

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_draw_text_vertical()");
	}
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);
    if (pch_text == NULL) { return 1; }
    g_return_val_if_fail (rect != NULL, -1);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);

    /* Get the default renderer for the screen, and set it up for drawing  */
    renderer = gdk_pango_renderer_get_default (gtk_widget_get_screen (GTK_WIDGET(graph)));
    gdk_pango_renderer_set_drawable (GDK_PANGO_RENDERER (renderer), GTK_WIDGET(graph)->window);
    gdk_pango_renderer_set_gc (GDK_PANGO_RENDERER (renderer), priv->title_gc);

    context = gtk_widget_get_pango_context (GTK_WIDGET(graph));

    layout = pango_layout_new (context);
    pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);
    pango_layout_set_markup (layout, pch_text, -1);

    pango_matrix_rotate (&matrix, 90.0);
    pango_context_set_matrix (context, &matrix);

    pango_layout_context_changed (layout);

                             /* xy switched due to rotate func */
    pango_layout_get_pixel_size (layout,  &rect->height, &rect->width);  
    if (priv->plot_box.height > rect->height ) {
		y_pos = rect->y + ((priv->plot_box.height - rect->height) / 2);
    } else {
		y_pos = ((priv->page_box.height - rect->height) / 2);    	 
	}

    if (glg_flag_debug) { 
		g_debug ("Vert:TextBox: y_pos=%d,  x=%d, y=%d, cx=%d, cy=%d", 
					y_pos, rect->x, rect->y, rect->width, rect->height);
    }

    gdk_draw_layout (GTK_WIDGET(graph)->window, priv->title_gc, rect->x, y_pos, layout);

    /* Clean up default renderer, since it is shared */
    gdk_pango_renderer_set_drawable (GDK_PANGO_RENDERER (renderer), NULL);
    gdk_pango_renderer_set_gc (GDK_PANGO_RENDERER (renderer), NULL);
    pango_context_set_matrix (context, NULL);

    /* free the objects we created */
    g_object_unref (layout);

    return rect->height;
}


/*
 * Draws the minor and major grid lines inside the current plot_area
 * returns -1 on error, or TRUE;
*/
static gint glg_line_graph_draw_grid_lines (GlgLineGraph *graph)
{
	GlgLineGraphPrivate *priv;
    gint        y_minor_inc = 0, y_pos = 0, y_index = 0;
    gint        y_major_inc = 0;
    gint        x_minor_inc = 0, x_pos = 0, x_index = 0;
    gint        x_major_inc = 0;
    gint        count_major = 0, count_minor = 0;
    GdkSegment *seg_minor = NULL;
    GdkSegment *seg_major = NULL;

	if (glg_flag_debug) {
		g_debug ("glg_line_graph_draw_grid_lines()");
	}
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
 
    count_major = priv->y_range.i_num_major;
    count_minor = priv->y_range.i_num_minor;
    y_minor_inc = priv->y_range.i_minor_inc;
    y_major_inc = priv->y_range.i_major_inc;

	if (glg_flag_debug) {
		g_debug
            ("Draw.Y-GridLines: count_major=%d, count_minor=%d, y_minor_inc=%d, y_major_inc=%d",
             count_major, count_minor, y_minor_inc, y_major_inc);
    }

    seg_minor = g_new0 (GdkSegment, count_minor + 8);
    seg_major = g_new0 (GdkSegment, count_major + 8);
    x_pos = priv->plot_box.width;
    y_pos = priv->plot_box.y;
    for (y_index = 0; y_index < count_minor; y_index++)
    {
        seg_minor[y_index].x1 = priv->plot_box.x;
        seg_minor[y_index].y1 = y_pos + (y_minor_inc * (y_index + 1));
        seg_minor[y_index].x2 = priv->plot_box.x + x_pos - 1;
        seg_minor[y_index].y2 = seg_minor[y_index].y1;
    }

    x_pos = priv->plot_box.width;
    y_pos = priv->plot_box.y;
    for (y_index = 0; y_index < count_major; y_index++)
    {
        seg_major[y_index].x1 = priv->plot_box.x;
        seg_major[y_index].y1 = y_pos + (y_major_inc * (y_index + 1));
        seg_major[y_index].x2 = priv->plot_box.x + x_pos - 1;
        seg_major[y_index].y2 = seg_major[y_index].y1;
    }

    if (priv->lgflags & GLG_GRID_MINOR_Y) {
        gdk_gc_set_line_attributes (priv->window_gc,
                                1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);
        gdk_draw_segments (GTK_WIDGET(graph)->window, priv->window_gc, seg_minor, count_minor - 1);
    }
    
    if (priv->lgflags & GLG_GRID_MAJOR_Y) {
        gdk_gc_set_line_attributes (priv->window_gc,
                                2, GDK_LINE_SOLID, GDK_CAP_ROUND, GDK_JOIN_BEVEL);
        gdk_draw_segments (GTK_WIDGET(graph)->window, priv->window_gc, seg_major, count_major - 1);
    }
    
    g_free (seg_minor);
    g_free (seg_major);

    count_major = priv->x_range.i_num_major;
    count_minor = priv->x_range.i_num_minor;
    x_minor_inc = priv->x_range.i_minor_inc;
    x_major_inc = priv->x_range.i_major_inc;

    if (glg_flag_debug) {
        g_debug
            ("Draw.X-GridLines: count_major=%d, count_minor=%d, x_minor_inc=%d, x_major_inc=%d",
             count_major, count_minor, x_minor_inc, x_major_inc);
    }

    seg_minor = g_new0 (GdkSegment, count_minor + 8);
    seg_major = g_new0 (GdkSegment, count_major + 8);
    x_pos = priv->plot_box.x;
    y_pos = priv->plot_box.height;
    for (x_index = 0; x_index < count_minor; x_index++)
    {
        seg_minor[x_index].x1 = priv->plot_box.x + (x_minor_inc * (x_index + 1));
        seg_minor[x_index].y1 = priv->plot_box.y +1;
        seg_minor[x_index].x2 = seg_minor[x_index].x1;
        seg_minor[x_index].y2 = priv->plot_box.y + y_pos;
    }

    x_pos = priv->plot_box.x;
    y_pos = priv->plot_box.height;
    for (x_index = 0; x_index < count_major; x_index++)
    {
        seg_major[x_index].x1 = priv->plot_box.x + (x_major_inc * (x_index + 1));
        seg_major[x_index].y1 = priv->plot_box.y +1;
        seg_major[x_index].x2 = seg_major[x_index].x1;
        seg_major[x_index].y2 = priv->plot_box.y + y_pos;
    }

    if (priv->lgflags & GLG_GRID_MINOR_X) {
        gdk_gc_set_line_attributes (priv->window_gc,
                                1, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_BEVEL);
        gdk_draw_segments (GTK_WIDGET(graph)->window, priv->window_gc, seg_minor, count_minor - 1);
    }
    
    if (priv->lgflags & GLG_GRID_MAJOR_X) {
        gdk_gc_set_line_attributes (priv->window_gc,
                                2, GDK_LINE_SOLID, GDK_CAP_ROUND, GDK_JOIN_BEVEL);
        gdk_draw_segments (GTK_WIDGET(graph)->window, priv->window_gc, seg_major, count_major - 1);
    }
    
    g_free (seg_minor);
    g_free (seg_major);

    return TRUE;
}

/*
 * Draw the chart x scale legend
*/
static void glg_line_graph_draw_x_grid_labels (GlgLineGraph *graph)
{
	GlgLineGraphPrivate *priv;
    gchar       ch_grid_label[GLG_MAX_BUFFER];
    gchar       ch_work[GLG_MAX_BUFFER];
    PangoLayout *layout = NULL;
    PangoTabArray *p_tabs = NULL;
    gint        x_adj = 0, x1_adj = 0, width = 0, height = 0, h_index = 0, x_scale = 0,
                cx = 0, cy = 0;

    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_draw_x_grid_labels()");
    }
	g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
    
    g_snprintf (ch_grid_label, GLG_MAX_BUFFER, "<small>%d</small>",
                priv->x_range.i_max_scale);
    layout = gtk_widget_create_pango_layout (GTK_WIDGET(graph), ch_grid_label);

    pango_layout_set_markup (layout, ch_grid_label, -1);
    pango_layout_get_pixel_size (layout, &width, &height);
    x_adj = width / 2;
    x1_adj = width / 4;

    g_snprintf (ch_grid_label, GLG_MAX_BUFFER, "<small>%s", "0");
    for (h_index = priv->x_range.i_inc_major_scale_by;
         h_index <= priv->x_range.i_max_scale;
         h_index += priv->x_range.i_inc_major_scale_by)
    {
        g_strlcpy (ch_work, ch_grid_label, GLG_MAX_BUFFER);
        g_snprintf (ch_grid_label, GLG_MAX_BUFFER, "%s\t%d", ch_work, h_index);
        if (h_index < 10)
        {
            x_scale++;
        }
    }
    g_strlcpy (ch_work, ch_grid_label, GLG_MAX_BUFFER);
    g_snprintf (ch_grid_label, GLG_MAX_BUFFER, "%s</small>", ch_work);

    pango_layout_set_markup (layout, ch_grid_label, -1);

    p_tabs = pango_tab_array_new (priv->x_range.i_num_major, TRUE);
    for (h_index = 0; h_index <= priv->x_range.i_num_major; h_index++)
    {
        gint        xbase = 0;

        if (h_index > x_scale)
        {
            xbase = (h_index * priv->x_range.i_major_inc);
        }
        else
        {
            xbase = (h_index * priv->x_range.i_major_inc) + x1_adj;
        }
        if (h_index == 0)
        {
            xbase = priv->x_range.i_major_inc + x1_adj;
        }
        pango_tab_array_set_tab (p_tabs, h_index, PANGO_TAB_LEFT, xbase);
    }
    pango_layout_set_tabs (layout, p_tabs);

    pango_layout_context_changed (layout);

    pango_layout_get_pixel_size (layout, &cx, &cy);

    if ( priv->page_box.width >  cx ) {
         gdk_draw_layout (GTK_WIDGET(graph)->window,
                              priv->scale_gc,
                              priv->plot_box.x - x_adj,
                              priv->plot_box.y + priv->plot_box.height, layout);
    }

    pango_tab_array_free (p_tabs);
    g_object_unref (layout);

    return;
}

/*
 * Draw the chart y scale legend
*/
static void glg_line_graph_draw_y_grid_labels (GlgLineGraph *graph)
{
	GlgLineGraphPrivate *priv;
    gchar       ch_grid_label[GLG_MAX_BUFFER];
    gchar       ch_work[GLG_MAX_BUFFER];
    PangoLayout *layout = NULL;
    gint        y_adj = 0, width = 0, height = 0, v_index = 0;

    if ( glg_flag_debug)
    {
        g_debug ("glg_line_graph_draw_y_grid_labels()");
    }
	g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
    
    g_snprintf (ch_grid_label, GLG_MAX_BUFFER, "<small>%d</small>",
                priv->y_range.i_max_scale);
    layout = gtk_widget_create_pango_layout (GTK_WIDGET(graph), ch_grid_label);

    pango_layout_set_markup (layout, ch_grid_label, -1);
    pango_layout_get_pixel_size (layout, &width, &height);
    y_adj = height / 2;

    g_snprintf (ch_grid_label, GLG_MAX_BUFFER, "<small>%d", priv->y_range.i_max_scale);
    for (v_index =
         priv->y_range.i_max_scale - priv->y_range.i_inc_major_scale_by;
         v_index > 0; v_index -= priv->y_range.i_inc_major_scale_by)
    {
        g_strlcpy (ch_work, ch_grid_label, GLG_MAX_BUFFER);
        g_snprintf (ch_grid_label, GLG_MAX_BUFFER, "%s\n%d", ch_work, v_index);
    }
    g_strlcpy (ch_work, ch_grid_label, GLG_MAX_BUFFER);
    g_snprintf (ch_grid_label, GLG_MAX_BUFFER, "%s</small>", ch_work);

    pango_layout_set_spacing (layout,
                              ((priv->y_range.i_major_inc - height) * PANGO_SCALE));
    pango_layout_set_alignment (layout, PANGO_ALIGN_RIGHT);
    pango_layout_set_markup (layout, ch_grid_label, -1);

    pango_layout_context_changed (layout);

    gdk_draw_layout (GTK_WIDGET(graph)->window,
                     priv->scale_gc,
                     priv->plot_box.x - (width * 1.1), priv->plot_box.y - y_adj, layout);

    g_object_unref (layout);

    return;
}


/*
 * Draws one data series points to chart
 * returns number of points processed
*/
static gint glg_line_graph_data_series_draw (GlgLineGraph *graph, PGLG_SERIES psd)
{
	GlgLineGraphPrivate *priv;
    gint        v_index = 0;
    GdkPoint   *point_pos = NULL;

    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_data_series_draw()");
    }
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), -1);
    g_return_val_if_fail (psd != NULL, -1);
    g_return_val_if_fail (psd->point_pos != NULL, -1);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);

    gdk_gc_set_rgb_fg_color (priv->series_gc, &psd->legend_color);
    gdk_gc_set_line_attributes (priv->series_gc, priv->series_line_width,
                                GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);

    point_pos = psd->point_pos;

/* trap first and only point */
    if (psd->i_point_count == 0)
    {
        return 0;
    }
    if (psd->i_point_count == 1)
    {
        point_pos[0].x = priv->plot_box.x;
        point_pos[0].y =
            (priv->plot_box.y + priv->plot_box.height) -
            ((psd->lg_point_dvalue[0] *
              (gdouble) ((gdouble) priv->plot_box.height /
                         (gdouble) priv->y_range.i_max_scale)));

        gdk_draw_arc (GTK_WIDGET(graph)->window, priv->series_gc, TRUE,
                      point_pos[0].x , 
                      point_pos[0].y , 
                      3, 3, 0, 360 * 64);
        return 1;
    }

    for (v_index = 0; v_index < psd->i_point_count; v_index++)
    {
        point_pos[v_index].x = priv->plot_box.x + (v_index * (priv->plot_box.width / priv->x_range.i_max_scale));
        point_pos[v_index].y = priv->plot_box.y + 
                               (priv->plot_box.height - 
                               (psd->lg_point_dvalue[v_index] * 
                               (gdouble) ((gdouble) priv->plot_box.height / (gdouble) priv->y_range.i_max_scale)));

            gdk_draw_arc (GTK_WIDGET(graph)->window, priv->series_gc, TRUE,
                          point_pos[v_index].x - 1,
                          point_pos[v_index].y - 2 , 
                          3., 3., 0, 360 * 64);
            gdk_draw_arc (GTK_WIDGET(graph)->window, priv->series_gc, FALSE,
                          point_pos[v_index].x - 1,
                          point_pos[v_index].y - 2 , 
                          3., 3., 0, 360 * 64);
    }

    gdk_draw_lines (GTK_WIDGET(graph)->window, priv->series_gc, point_pos, psd->i_point_count);

    return v_index;
}

/*
 * Draws all data series points to chart
 * returns number of series processed
*/
static gint glg_line_graph_data_series_draw_all (GlgLineGraph *graph, gboolean redraw_control)
{
	GlgLineGraphPrivate *priv;
    PGLG_SERIES  psd = NULL;
    GList      *data_sets = NULL;
    gint        v_index = 0;

    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_data_series_draw_all()");
    }
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
    

    data_sets = g_list_first (priv->lg_series);
    while (data_sets)
    {
        psd = data_sets->data;
        if (psd != NULL)
        {                       /* found */
            glg_line_graph_data_series_draw (graph, psd);
            v_index++;
        }
        data_sets = g_list_next (data_sets);
    }

    if (glg_flag_debug)
    {
        g_debug ("DrawAllDataSeries: series=%d", v_index);
    }

    return v_index;
}
/**
 * glg_line_graph_data_series_add_value:
 * @graph: pointer to a #GlgLineGraph widget
 * @i_series_number: The data series to add value to.
 * @y_value: The value to add.  Current range is 0.0 to y-max-scale
 * 
 * Add a single y value to the requested data series.
 * auto indexes the value if x-scale max is reached (appends to the end)
 * The X value is implied to be the current count of Y-values added.
 *
 * Returns: gboolean  TRUE if value was added, FALSE if add failed.
 */
extern gboolean glg_line_graph_data_series_add_value (GlgLineGraph *graph, gint i_series_number, gdouble y_value)
{
	GlgLineGraphPrivate *priv;
    PGLG_SERIES  psd = NULL;
    GList      *data_sets = NULL;
    gint        v_index = 0;
    gboolean    b_found = FALSE;

    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_data_series_add_value()");
    }
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
    
    data_sets = g_list_first (priv->lg_series);
    while (data_sets)
    {
        psd = data_sets->data;
        if (psd->i_series_id == i_series_number)
        {                       /* found */
            b_found = TRUE;
            break;
        }
        data_sets = g_list_next (data_sets);
    }

    if (!b_found)
    {
        g_message ("glg_line_graph_data_series_add_value(%d): Invalid data series number",
                   i_series_number);
        return FALSE;
    }

    if (y_value > priv->y_range.i_max_scale)
    {
        y_value = (gdouble) priv->y_range.i_max_scale;
    }

    if (psd->i_point_count == psd->i_max_points + 1)
    {
        for (v_index = 0; v_index < psd->i_max_points; v_index++)
        {
            psd->lg_point_dvalue[v_index] = psd->lg_point_dvalue[v_index + 1];
        }
        psd->lg_point_dvalue[psd->i_max_points] = y_value;
    }
    else
    {
        psd->lg_point_dvalue[psd->i_point_count++] = y_value;
    }

    psd->d_max_value = MAX (y_value, psd->d_max_value);
    psd->d_min_value = MIN (y_value, psd->d_min_value);

    priv->i_points_available = MAX (priv->i_points_available, psd->i_point_count);

    /* record current time with data points */
    if (psd->i_series_id == priv->i_num_series - 1)
    {
        GList      *gl_remove = NULL;

        if (g_list_length (priv->lg_series_time) == psd->i_max_points +1 )
        {
            gl_remove = g_list_first (priv->lg_series_time);
            priv->lg_series_time =
                g_list_remove_all (priv->lg_series_time, gl_remove->data);
        }
        priv->lg_series_time =
            g_list_append (priv->lg_series_time, GINT_TO_POINTER ((time_t) time (NULL)));
            
    }

    if (glg_flag_debug)
    {
        g_debug
         ("  ==>DataSeriesAddValue: series=%d, value=%3.1lf, index=%d, count=%d, max_pts=%d",
          i_series_number, y_value, v_index, psd->i_point_count, psd->i_max_points);
    }

    return TRUE;
}

/*
 * A shutdown routine
 * destroys all the data series and any assocaited dynamic data
*/
static gint glg_line_graph_data_series_remove_all (GlgLineGraph *graph)
{
	GlgLineGraphPrivate *priv;
    PGLG_SERIES  psd = NULL;
    GList      *data_sets = NULL;
    gint        i_count = 0;

    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_data_series_remove_all()");
    }
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
    
    data_sets = g_list_first (priv->lg_series);
    while (data_sets)
    {
        psd = data_sets->data;
        g_free (psd->lg_point_dvalue);
        g_free (psd->point_pos);
        g_free (psd);
        data_sets = g_list_next (data_sets);
        i_count++;
    }
    g_list_free (priv->lg_series);
    g_list_free (priv->lg_series_time);
    priv->lg_series = NULL;
    priv->lg_series_time = NULL;
    priv->i_num_series = 0;
    priv->i_points_available = 0;    

    if (glg_flag_debug)
    {
        g_debug ("  ==>DataSeriesRemoveAll: number removed=%d", i_count);
    }

    return TRUE;
}

/**
 * glg_line_graph_data_series_add:
 * @graph: pointer to a #GlglineGraph widget
 * @pch_legend_text: The name of the data series
 * @pch_color_text: A string containing the line color to be used.
 *
 * Allocates space for another data series of y-values and returns the 
 * series number of this dataset added which you must keep track of
 * to add values.
 *
 * Returns: gint  The series number of this dataset added ( range 0 thru n )
 */
extern gint glg_line_graph_data_series_add (GlgLineGraph *graph, const gchar *pch_legend_text, const gchar *pch_color_text)
{
	GlgLineGraphPrivate *priv;
    PGLG_SERIES  psd = NULL;

    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_data_series_add()");
    }
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), FALSE);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);

    g_return_val_if_fail (pch_legend_text != NULL, -1);
    g_return_val_if_fail (pch_color_text != NULL, -1);

    psd = (PGLG_SERIES) g_new0 (GLG_SERIES, 1);
    g_return_val_if_fail (psd != NULL, -1);

    psd->lg_point_dvalue = (gdouble *) g_new0 (gdouble, (priv->x_range.i_max_scale + 4));
    g_return_val_if_fail (psd->lg_point_dvalue != NULL, -1);

    psd->point_pos = g_new0 (GdkPoint, (priv->x_range.i_max_scale + 4));
    g_return_val_if_fail (psd->point_pos != NULL, -1);

    g_snprintf (psd->ch_legend_text, sizeof (psd->ch_legend_text), "%s", pch_legend_text);
    
    /*
     * we position x to ticks onlys, 
     * so force chart to scroll at maximum ticks vs value */
    psd->i_max_points = priv->x_range.i_max_scale;;
    
    gdk_color_parse (pch_color_text, &psd->legend_color);
    g_snprintf (psd->ch_legend_color, sizeof (psd->ch_legend_color), "%s", pch_color_text);
    psd->cb_id = GLG_SERIES_ID;

    priv->lg_series = g_list_append (priv->lg_series, psd);
    psd->i_series_id = priv->i_num_series++;

    if (glg_flag_debug)
    {
        g_debug ("  ==>DataSeriesAdd: series=%d, max_pts=%d",
                 psd->i_series_id, psd->i_max_points);
    }

    return psd->i_series_id;
}

/*
 * Toggle the legend function on off
 * "button-press-event"
 * secret decoder function: button 3 enables, button 2, for debug messages toggle
*/
static gboolean glg_line_graph_button_press_event (GtkWidget * widget, GdkEventButton * ev)
{
    GlgLineGraphPrivate *priv;

  
    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_button_press_event_cb()");
    }
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(widget), FALSE);

	priv = GLG_LINE_GRAPH_GET_PRIVATE (widget);
    
    if ((ev->type & GDK_BUTTON_PRESS) && (ev->button == 1))
    {
        gdk_window_get_pointer (ev->window, &priv->mouse_pos.x, &priv->mouse_pos.y, &priv->mouse_state);
        glg_line_graph_redraw (GLG_LINE_GRAPH(widget));        /* point select action */
        return TRUE;
    }
    if ((ev->type & GDK_BUTTON_PRESS) && (ev->button == 2) && priv->b_mouse_onoff)
    {
        glg_flag_debug = glg_flag_debug ? FALSE : TRUE; 
        return TRUE;        
    }
    if ((ev->type & GDK_BUTTON_PRESS) && (ev->button == 3))
    {
        priv->b_mouse_onoff = priv->b_mouse_onoff ? FALSE : TRUE; 
        return TRUE;        
    }

    return FALSE;
}

/*
 * Track the mouse pointer position
 * "motion-notify-event"
 */



static void glg_line_graph_destroy (GtkObject *object)
{
  GlgLineGraphPrivate *priv = NULL;
  GtkWidget       *widget = NULL;

    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_destroy(enter)");
    }

  g_return_if_fail (object != NULL);
  
  widget = GTK_WIDGET( object );

  g_return_if_fail ( GLG_IS_LINE_GRAPH(widget));

  priv = GLG_LINE_GRAPH_GET_PRIVATE (widget);
  g_return_if_fail ( priv != NULL );
  
  if ( priv->window_gc )  /* avoid multiple destroys */
  {
      glg_line_graph_data_series_remove_all ( GLG_LINE_GRAPH( widget ) );

      g_free(priv->x_label_text);
      g_free(priv->y_label_text);
      g_free(priv->page_title_text);
      priv->x_label_text = NULL;
      priv->y_label_text = NULL;
      priv->page_title_text = NULL;

      g_object_unref ( priv->window_gc );
      g_object_unref ( priv->box_gc );
      g_object_unref ( priv->scale_gc );
      g_object_unref ( priv->title_gc );
      g_object_unref ( priv->series_gc );

      priv->window_gc = NULL;
      priv->box_gc = NULL;
      priv->scale_gc = NULL;
      priv->title_gc = NULL;
      priv->series_gc = NULL;

      if (GTK_OBJECT_CLASS (glg_line_graph_parent_class)->destroy != NULL)
      {
         (*GTK_OBJECT_CLASS (glg_line_graph_parent_class)->destroy) (object);
      }
  }
    if ( glg_flag_debug)
    {
        g_debug ("glg_line_graph_destroy(exit)");
    }

  return;
}

/**
 * glg_line_graph_chart_set_elements:
 * @graph: pointer to a #GlglineGraph widget
 * @element: An or'ed list of #GLGElementID indicating what graph elements should be drawn
 * to the screen. 
 *
 * Controls whether the grids, labels, tooltip, and titles will appear on the chart.
 * All graphs are created with the following defaults; 
 * %GLG_TOOLTIP | 
 * %GLG_GRID_LABELS_X | %GLG_GRID_LABELS_Y | %GLG_TITLE_T | %GLG_TITLE_X | %GLG_TITLE_Y | 
 * %GLG_GRID_LINES | %GLG_GRID_MINOR_X | %GLG_GRID_MAJOR_X | %GLG_GRID_MINOR_Y | %GLG_GRID_MAJOR_Y 
 */
extern void glg_line_graph_chart_set_elements ( GlgLineGraph *graph, GLGElementID element)
{
	GlgLineGraphPrivate *priv = NULL;
	
    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_chart_set_elements()");
    }
	g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));

  	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
  	g_return_if_fail ( priv != NULL );

    priv->lgflags |= element;
}

/**
 * glg_line_graph_chart_get_elements:
 * @graph: pointer to a GlglineGraph widget
 * 
 * Retrieves the current draw setting for the graph.  AND it with the desired value to test
 * if what your after is present.
 *
 * Returns: gint containing all current draw flags as defined in #GLGElementID
 */
extern GLGElementID glg_line_graph_chart_get_elements ( GlgLineGraph *graph)
{
    GlgLineGraphPrivate *priv = NULL;
    
    
    if (glg_flag_debug)
    {
        g_debug ("glg_line_graph_chart_get_elements()");
    }
	g_return_val_if_fail ( GLG_IS_LINE_GRAPH(graph), 0);

  	priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
  	g_return_val_if_fail ( priv != NULL, 0 );

    return priv->lgflags;
}


static void glg_line_graph_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
  GlgLineGraphPrivate *priv = NULL;
  GlgLineGraph *graph = NULL;
  gint  i_value = 0;


  if (glg_flag_debug)
  {
      g_debug ("glg_line_graph_set_property()");
  }  
  g_return_if_fail ( object != NULL);    

  graph = GLG_LINE_GRAPH (object);
  g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));

  priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
  g_return_if_fail ( priv != NULL);
  
  switch (prop_id)
    {
    case PROP_GRAPH_TITLE:
      glg_line_graph_chart_set_text (graph, GLG_TITLE_T, g_value_get_string(value) );
      break;      
    case PROP_GRAPH_TITLE_X:
      glg_line_graph_chart_set_text (graph, GLG_TITLE_X, g_value_get_string(value) );
      break;      
    case PROP_GRAPH_TITLE_Y:
      glg_line_graph_chart_set_text (graph, GLG_TITLE_Y, g_value_get_string(value) );
      break;      
    case PROP_GRAPH_LINE_WIDTH:
      priv->series_line_width = g_value_get_int ( value );
      break;
    case PROP_GRAPH_ELEMENTS:
      priv->lgflags |= g_value_get_int ( value );    
      break;
	case PROP_GRAPH_TITLE_COLOR:
      glg_line_graph_chart_set_color (graph, GLG_TITLE,  g_value_get_string(value));
	  break;      
	case PROP_GRAPH_SCALE_COLOR:
      glg_line_graph_chart_set_color (graph, GLG_SCALE,  g_value_get_string(value));
	  break;      
	case PROP_GRAPH_CHART_COLOR:
      glg_line_graph_chart_set_color (graph, GLG_CHART,  g_value_get_string(value));
	  break;      
	case PROP_GRAPH_WINDOW_COLOR:
      glg_line_graph_chart_set_color (graph, GLG_WINDOW, g_value_get_string(value));	
	  break;    
	case PROP_TICK_MINOR_X:
	  i_value = g_value_get_int ( value );
	  priv->x_range.i_inc_minor_scale_by = i_value;
	  priv->x_range.i_num_minor = priv->x_range.i_max_scale / i_value;
	  break;	    
	case PROP_TICK_MAJOR_X:
	  i_value = g_value_get_int ( value );
      priv->x_range.i_inc_major_scale_by = i_value;	  	
      priv->x_range.i_num_major = priv->x_range.i_max_scale / i_value;
	  break;	    
	case PROP_SCALE_MINOR_X:
	  i_value = g_value_get_int ( value );
	  priv->x_range.i_min_scale = i_value;	
	  break;	    
	case PROP_SCALE_MAJOR_X:
	  i_value = g_value_get_int ( value );
	  if (priv->x_range.i_max_scale) {
		  g_message ("Set Properties Failed: Cannot set ranges more than once, range already set!");
		  break;
	  }	  	  
	  priv->x_range.i_max_scale = i_value;	
      priv->x_range.i_num_minor = i_value / priv->x_range.i_inc_minor_scale_by;
      priv->x_range.i_num_major = i_value / priv->x_range.i_inc_major_scale_by; 
	  break;	    
	case PROP_TICK_MINOR_Y:
	  i_value = g_value_get_int ( value );
	  priv->y_range.i_inc_minor_scale_by = i_value;
	  priv->y_range.i_num_minor = priv->y_range.i_max_scale / i_value;
	  break;	    
	case PROP_TICK_MAJOR_Y:
	  i_value = g_value_get_int ( value );
      priv->y_range.i_inc_major_scale_by = i_value;	  	
      priv->y_range.i_num_major = priv->y_range.i_max_scale / i_value;
	  break;	    
	case PROP_SCALE_MINOR_Y:
	  i_value = g_value_get_int ( value );
	  priv->y_range.i_min_scale = i_value;	
	  break;	    
	case PROP_SCALE_MAJOR_Y:
	  i_value = g_value_get_int ( value );
	  priv->y_range.i_max_scale = i_value;	
      priv->y_range.i_num_minor = i_value / priv->y_range.i_inc_minor_scale_by;
      priv->y_range.i_num_major = i_value / priv->y_range.i_inc_major_scale_by; 
	  break;	    

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
    
    return;
}

static void glg_line_graph_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
  GlgLineGraphPrivate *priv = NULL;
  GlgLineGraph *graph = NULL;

  if (glg_flag_debug)
  {
      g_debug ("glg_line_graph_get_property()");
  }
  
  g_return_if_fail ( object != NULL);    

  graph = GLG_LINE_GRAPH (object);
  g_return_if_fail ( GLG_IS_LINE_GRAPH(graph));

  priv = GLG_LINE_GRAPH_GET_PRIVATE (graph);
  g_return_if_fail ( priv != NULL);
  
  switch (prop_id)
    {
    case PROP_GRAPH_ELEMENTS:
          g_value_set_int (value, priv->lgflags);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
    
    return;
}

/*
 * GOBJECT Marshalling routines
 * - required by class init and point-selected-signal 
*/

#ifdef G_ENABLE_DEBUG
	#define g_marshal_value_peek_double(v)   g_value_get_double (v)
#else /* !G_ENABLE_DEBUG */
	#define g_marshal_value_peek_double(v)   (v)->data[0].v_double
#endif /* !G_ENABLE_DEBUG */

static void _glg_marshal_VOID__DOUBLE_DOUBLE_DOUBLE_DOUBLE (GClosure     *closure,
                                                      GValue       *return_value,
                                                      guint         n_param_values,
                                                      const GValue *param_values,
                                                      gpointer      invocation_hint,
                                                      gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__DOUBLE_DOUBLE_DOUBLE_DOUBLE) (gpointer     data1,
                                                                  gdouble      arg_1,
                                                                  gdouble      arg_2,
                                                                  gdouble      arg_3,
                                                                  gdouble      arg_4,
                                                                  gpointer     data2);
  register GMarshalFunc_VOID__DOUBLE_DOUBLE_DOUBLE_DOUBLE callback;
  register GCClosure *cc = (GCClosure*) closure;
  register gpointer data1, data2;

  g_return_if_fail (n_param_values == 5);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__DOUBLE_DOUBLE_DOUBLE_DOUBLE) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_double (param_values + 1),
            g_marshal_value_peek_double (param_values + 2),
            g_marshal_value_peek_double (param_values + 3),
            g_marshal_value_peek_double (param_values + 4),
            data2);
}
