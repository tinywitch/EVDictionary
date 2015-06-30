/* 
 * File:   main_controller.h
 * Author: sakura
 *
 * Created on May 5, 2015, 2:25 PM
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "main_library.h"
#define search mainapp->search
#define add mainapp->add
#define edit mainapp->edit
#define delete  mainapp->delete
#define help mainapp->help
#define anhviet mainapp->anhviet
#define vietanh mainapp->vietanh
#define about mainapp->about
#define listview mainapp->listview
#define entry mainapp->entry
#define buffer mainapp->buffer
#define view mainapp->view
#define btree mainapp->btree
#define selection mainapp->selection

void main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *hbox;
	GtkWidget *vbox1,*vbox2,*vbox3;
	GtkWidget *hbox_vbox3;

	GtkWidget *listview_sw;
	App *mainapp = g_slice_new(App);
	gtk_rc_parse("gtkrc");
	btinit();
	btree = btopn("Data/Anhviet.dict",0,0);
	gtk_init(&argc,&argv);

	/* 
	* Tao main window
	*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window),"English - Vietnamese Dictionary");
	gtk_window_set_default_size(GTK_WINDOW(window),750,520);
	gtk_container_set_border_width(GTK_CONTAINER(window),5);
	
	/*
	* Signal Window
	*/	
	g_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),window);

	/*
	* Tao box 
	*/	
	hbox = gtk_hbox_new(FALSE,5);
	vbox1 = gtk_vbox_new(FALSE,5);
	vbox2 = gtk_vbox_new(FALSE,25);
	vbox3 = gtk_vbox_new(FALSE,5);
	hbox_vbox3 = gtk_hbox_new(FALSE,5);

	/*
	* Them entry vao vbox 1
	*/	
	entry = gtk_entry_new_with_max_length(200);
	GtkWidget *firstLabel = gtk_label_new("Word:");
	gtk_box_pack_start(GTK_BOX(vbox1),firstLabel,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox1),entry,FALSE,FALSE,5);
	
	// Phim Enter de tra nghia tu
	g_signal_connect(GTK_OBJECT(entry),"activate",G_CALLBACK(search_dict),mainapp);
	
	// Cap nhat tu can tra trong box de ra dung nghia mong muon
	g_signal_connect(GTK_OBJECT(entry),"changed",G_CALLBACK(change_text),mainapp);

	// Su kien an phim tab de tu dong dien vao box
	g_signal_connect(GTK_OBJECT(entry),"key-press-event",G_CALLBACK(auto_complete),mainapp);

	/*
	* Them listview vao vbox 1
	*/
	listview = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listview),FALSE);
	init_list(listview);
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(listview));
	g_signal_connect(selection,"changed", G_CALLBACK(on_changed),mainapp);
	listview_sw = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(listview_sw),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(listview_sw),GTK_SHADOW_ETCHED_IN);
	gtk_container_add(GTK_CONTAINER(listview_sw),listview);
	gtk_box_pack_start(GTK_BOX(vbox1),listview_sw,TRUE,TRUE,5);
	/*
	* Them listview vao vbox 2
	*/
	GtkWidget *secondlabel=gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox2),secondlabel,FALSE,FALSE,0);

    GtkWidget *searchIMG, *addIMG, *editIMG, *deleteIMG, *helpIMG, *infoIMG;
    searchIMG = gtk_image_new_from_file("Resource/search.png");
    addIMG = gtk_image_new_from_file("Resource/insert.png");
    editIMG = gtk_image_new_from_file("Resource/edit.png");
    deleteIMG = gtk_image_new_from_file("Resource/delete.png");
    helpIMG = gtk_image_new_from_file("Resource/help.png");
    infoIMG = gtk_image_new_from_file("Resource/information.png");

    search = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(search),searchIMG);
    gtk_box_pack_start(GTK_BOX(vbox2),search,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(search),"clicked",G_CALLBACK(search_dict), (gpointer)mainapp);

    add = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(add),addIMG);
    gtk_box_pack_start(GTK_BOX(vbox2),add,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(add),"clicked",G_CALLBACK(add_dialog),(gpointer)mainapp);

    edit = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(edit),editIMG);
    gtk_box_pack_start(GTK_BOX(vbox2),edit,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(edit),"clicked",G_CALLBACK(edit_dialog),(gpointer)mainapp);

    delete = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(delete),deleteIMG);
    gtk_box_pack_start(GTK_BOX(vbox2),delete,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(delete),"clicked",G_CALLBACK(delete_dialog),(gpointer)mainapp);

    help = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(help),helpIMG);
    gtk_box_pack_start(GTK_BOX(vbox2),help,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(help),"clicked",G_CALLBACK(help_dialog),(gpointer)mainapp);

    about = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(about),infoIMG);
    gtk_box_pack_end(GTK_BOX(vbox2),about,FALSE,FALSE,0);

    /*search=gtk_button_new_from_stock(GTK_STOCK_FIND);
    gtk_box_pack_start(GTK_BOX(vbox2),search,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(search),"clicked",G_CALLBACK(search_dict), (gpointer)mainapp);
 
    add=gtk_button_new_from_stock(GTK_STOCK_ADD);
    gtk_box_pack_start(GTK_BOX(vbox2),add,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(add),"clicked",G_CALLBACK(add_dialog),(gpointer)mainapp);
    
    edit=gtk_button_new_from_stock(GTK_STOCK_EDIT );
    gtk_box_pack_start(GTK_BOX(vbox2),edit,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(edit),"clicked",G_CALLBACK(edit_dialog),(gpointer)mainapp);

    delete=gtk_button_new_from_stock(GTK_STOCK_DELETE);
    gtk_box_pack_start(GTK_BOX(vbox2),delete,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(delete),"clicked",G_CALLBACK(delete_dialog),(gpointer)mainapp);

    help=gtk_button_new_from_stock(GTK_STOCK_HELP);
    gtk_box_pack_start(GTK_BOX(vbox2),help,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(help),"clicked",G_CALLBACK(help_dialog),(gpointer)mainapp);

    about=gtk_button_new_from_stock(GTK_STOCK_DIALOG_INFO);
    gtk_box_pack_end(GTK_BOX(hbox_vbox3),about,FALSE,FALSE,0);*/
    
   	/*
	* Them listview vao vbox 3
	*/
    anhviet=gtk_label_new("Mean");
    gtk_box_pack_start(GTK_BOX(hbox_vbox3),anhviet,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox3),hbox_vbox3,FALSE,TRUE,5);

    view=gtk_text_view_new();
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (view), GTK_WRAP_WORD);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view),FALSE);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view),10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view),10); 
    
    buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

    GtkWidget *scrolled_win=gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
    gtk_container_add(GTK_CONTAINER(scrolled_win),view);
    gtk_box_pack_start(GTK_BOX(vbox3),scrolled_win,TRUE,TRUE,5);
    
    
    //THEM vbox vao hbox;
    gtk_box_pack_start(GTK_BOX(hbox),vbox1,FALSE,TRUE,5);
    gtk_box_pack_start(GTK_BOX(hbox),vbox2,FALSE,TRUE,5);
    gtk_box_pack_start(GTK_BOX(hbox),vbox3,TRUE,TRUE,5);
    
    GtkWidget *about_dialog=gtk_about_dialog_new();
    g_signal_connect(G_OBJECT(about),"clicked",G_CALLBACK(set_dialog),(gpointer)window);
    set_list_view(mainapp,NULL);
    gtk_container_add(GTK_CONTAINER(window),hbox);
    gtk_widget_show_all(window);
    gtk_main();
    btcls(btree);

}