#ifndef _MICROMENU_H_
#define _MICROMENU_H_


#include <stdlib.h>

#include <cfg/compiler.h>
#include <cpu/attr.h>     /* CPU_HARVARD */
#include <cpu/pgm.h>

#if CPU_HARVARD
#include <avr/pgmspace.h>
#include <avr/io.h>
#endif




#define MENU_ITEM_STORAGE				PROGMEM
//#define MENU_ITEM_STORAGE				PGM_ATTR

#if CPU_HARVARD
#define MENU_ITEM_READ_POINTER(addr)	(void*)pgm_read_word(addr)
#else
#define MENU_ITEM_READ_POINTER(addr)	*addr
#endif

#define NULL_FUNC 						(void*)0	/** Causes reset if called !!! **/


/** Type define for a menu item. Menu items should be initialized via the helper
 *  macro \ref MENU_ITEM(), not created from this type directly in user-code.
 */
typedef const struct menu_item {
	const struct menu_item *next; 		/**< Pointer to the next menu item of this menu item */
	const struct menu_item *prev; 		/**< Pointer to the prev menu item of this menu item */
	const struct menu_item *parent; 	/**< Pointer to the parent menu item of this menu item */
	const struct menu_item *child; 		/**< Pointer to the child menu item of this menu item */

	void (*next_callback)(void); 		/**< Pointer to the optional menu-specific select callback of this menu item */
	void (*prev_callback)(void); 		/**< Pointer to the optional menu-specific select callback of this menu item */
	void (*enter_callback)(void); 		/**< Pointer to the optional menu-specific enter callback of this menu item */
	void (*display_callback)(void); 	/**< Pointer to the optional menu-specific enter callback of this menu item */

	const char text[]; 					/**< Menu item text to pass to the menu display callback function */
} menu_item_t;

/*
typedef const struct menu_list {
	const *menu_items;
	const uint8_t len;
} menu_list_t;
*/

extern void (*next_func)(void);
extern void (*prev_func)(void);
extern void (*enter_func)(void);
extern void (*display_func)(void);

extern void (*lcdm_write_function)(const char* text);

extern menu_item_t* current_menu_item;



/** Creates a new menu item entry with the specified links and callbacks.
 *
 *  \param[in] Name      Name of the menu entry, must be unique.
 *  \param[in] Next      Name of the next linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Previous  Name of the previous linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Parent    Name of the parent linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Child     Name of the child linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] SelectFunc  Function callback to execute when the menu item is selected, or \c NULL for no callback.
 *  \param[in] EnterFunc   Function callback to execute when the menu item is entered, or \c NULL for no callback.
 */
#define MENU_ITEM(name, next, prev, parent, child, next_func, prev_func, enter_func, display_func, text) \
	extern menu_item_t MENU_ITEM_STORAGE next;     \
	extern menu_item_t MENU_ITEM_STORAGE prev;     \
	extern menu_item_t MENU_ITEM_STORAGE parent;   \
	extern menu_item_t MENU_ITEM_STORAGE child;  \
	menu_item_t MENU_ITEM_STORAGE name = {&next, &prev, &parent, &child, next_func, prev_func, enter_func, display_func, text}


/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu parent. */
#define MENU_PARENT         MENU_ITEM_READ_POINTER(&lcdm_get_current_menu()->parent)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu child. */
#define MENU_CHILD          MENU_ITEM_READ_POINTER(&lcdm_get_current_menu()->child)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
#define MENU_NEXT           MENU_ITEM_READ_POINTER(&lcdm_get_current_menu()->next)

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
#define MENU_PREV           MENU_ITEM_READ_POINTER(&lcdm_get_current_menu()->prev)


/** Null menu entry, used in \ref MENU_ITEM() definitions where no menu link is to be made. */
extern menu_item_t MENU_ITEM_STORAGE NULL_MENU;


/** Retrieves the currently selected meny item.
 *
 *  \return Pointer to the currently selected meny item.
 */
menu_item_t* lcdm_get_current_menu(void);

menu_item_t* lcdm_get_parent_menu(void);

menu_item_t* lcdm_get_child_menu(void);

/** Navigates to an absolute or relative menu entry.
 *
 * \param[in] NewMenu  Pointer to the absolute menu item to select, or one of \ref MENU_PARENT,
 *                     \ref MENU_CHILD, \ref MENU_NEXT or \ref MENU_PREVIOUS for relative navigation.
 */
void lcdm_navigate(menu_item_t* const new_menu);


/** Configures the menu text write callback function, fired for all menu items. Within this callback
 *  function the user should implement code to display the current menu text stored in \ref MENU_ITEM_STORAGE
 *  memory space.
 *
 *  \ref WriteFunc  Pointer to a callback function to execute for each selected menu item.
 */
void lcdm_set_default_write_callback(void (*write_func)(const char* text));


/** Enters the currently selected menu item, running its configured callback function (if any). */
void lcdm_enter_current_item(void);


/*
const char* lcdm_get_parent_menu_text(menu_item_t* const cur_menu);

const char* lcdm_get_child_menu_text(menu_item_t* const cur_menu);
*/


#endif
