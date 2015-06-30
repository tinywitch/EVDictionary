/* 
 * File:   main_library.h
 * Author: sakura
 *
 * Created on May 4, 2015, 3:50 PM
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "dict_generate.h"
#include "soundex.h"

typedef struct app
{
	GtkWidget *search;
	GtkWidget *add, *edit, *delete, *help, *anhviet, *vietanh, *about, *listview, *entry, *view;
	GtkTreeSelection *selection;
	GtkTextBuffer *buffer;
	BTA *btree;
} App;

void init_list(GtkWidget *listview)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("listview Item",renderer,"text",0,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(listview),column);
	store = gtk_list_store_new(1,G_TYPE_STRING);
	gtk_tree_view_set_model (GTK_TREE_VIEW(listview),GTK_TREE_MODEL(store));
	g_object_unref(store);
}

void set_dialog(GtkWidget *button)
{
	GtkWidget *dialog = gtk_about_dialog_new();
	GdkPixbuf *logo;
	GError *error = NULL;
	const gchar *authors[] =
			{
				"Lai Thuy Ninh","Email: thuyninh.esc@gmail.com",NULL	
			};
	logo = gdk_pixbuf_new_from_file("Resource/logo.png",&error);
	/* Set the application logo or handle the error. */
	if(error == NULL)
		gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog),logo);
	else
	{
		if (error->domain == GDK_PIXBUF_ERROR)
			g_print ("GdkPixbufError: %s \n", error->message);
		else if (error->domain == G_FILE_ERROR)
			g_print ("GFileError: %s \n",error->message);
		else 
			g_print ("An error in the domain: %d has occurred \n", error->domain);
		g_error_free(error);		
	}

	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"C Project");
	gtk_window_set_default_size((GtkWindow*) dialog,600,600);
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), authors);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	return ;
}

static void add_to_list(GtkWidget *listview, const gchar *string)
{
	GtkListStore *store;
	GtkTreeIter iter;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(listview)));
	gtk_list_store_append(store,&iter);
	gtk_list_store_set(store, &iter, 0, string, -1);
}

void set_list_view(App *mainapp, char *position)
{
	char *word = (char*) malloc(200);
	char *mean = (char*) malloc(MAX_LEN);
	int rsize;
	int i = 0;
	GtkListStore *store;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(mainapp->listview)));
	gtk_list_store_clear(store);
	if (position==NULL)
	{
		btpos(mainapp->btree,1);
		while(btseln(mainapp->btree, word, mean, MAX_LEN, &rsize) == 0)
		{
			i++;
			if (i==50)
				break;
			add_to_list(mainapp->listview,word);
		}
	}
	else
	{
		add_to_list(mainapp->listview,position);
		while (btseln(mainapp->btree, word, mean, MAX_LEN, &rsize) == 0)
		{
			if (strcmp(position, word) <= 0 )
			{
				i++;
				if (i==50)
					break;
				add_to_list(mainapp->listview, word);
			}
		}
	}
	free(word);
	free(mean);
}

void search_dict(GtkWidget *search, gpointer data)
{
	App *mainapp = (App*) data;
	char *find, *find_soundex, *ptr;
	const char *word2 = gtk_entry_get_text((GtkEntry*) mainapp->entry);
	word2 = strlwr((char*) word2);
	char *word = strdup(word2);
	char *result, *result2, *mean2;
	result2 = (char*) malloc (MAX_LEN);
	BTA *btsoundex = btopn("Data/Anhvietsoundex.dict",0,0);

	find = search_btree(mainapp->btree, (char*) word); //Tim xem co trong tu dien khong
	if (find == NULL) // neu tu nhap vao trong box khong co trong tu dien
	{
		// thi tim trong btree soundex tu gan dung voi tu muon tra
		find_soundex = search_btree(btsoundex,soundex(word)); 
		if (find_soundex == NULL) // Neu khong tim thay tu nao cung ma soundex
			gtk_text_buffer_set_text(mainapp->buffer,"Not found\n",-1);
		else
		{
			// tim tu o vi tri tiep theo trong soundex btree
			result = next_position_in_soundex_code(word,find_soundex); 
			if (result!=NULL)
			{
				// tra ket qua tim bang btree soundex cho bien word roi tra nghia
				strcpy(result2,"Not found. Suggestion:\n");
				result2[strlen(result2)]='\0';
				mean2 = search_btree(mainapp->btree,result);
				strcpy(result2+strlen(result2),mean2);
				result2[strlen(result2)]='\0';
				gtk_text_buffer_set_text(mainapp->buffer,result2,-1);
			}
			else 
				gtk_text_buffer_set_text(mainapp->buffer,"Not found\n",-1);
		}
	}
	else
		gtk_text_buffer_set_text(mainapp->buffer,find,-1);
	btcls(btsoundex);
}

char *comparex(char *s1, char *s2)
{
	char *s = (char *) malloc(200);
	int n = strlen(s1);
	int i;
	for (i=0;i<=n;i++)
	{
		if(s1[i] == s2[i])
			s[i]=s1[i];
		else
			break;
	}
	s[i] = '\0';
	return s;
}

char *check_character(BTA *btree, char *word)
{
	char *word1 = (char *) malloc(200);
	char *mean = (char *) malloc(MAX_LEN);
	btpos(btree,1);
	int rsize;
	while (btseln(btree,word1,mean,MAX_LEN,&rsize) == 0)
	{
		if(strncmp(word1,word,strlen(word)) == 0)
			return word1;
		if (strncmp(word1,word,strlen(word)) >0 )
			return NULL;
	}
	return NULL;
}

void change_text(GtkWidget *entry, gpointer data)
{
	App *mainapp = (App *) data;
	btpos(mainapp->btree,1);
	const char *word = gtk_entry_get_text((GtkEntry *) mainapp->entry);
	word = strlwr((char*) word);
	gtk_entry_set_text((GtkEntry*) entry,word);
	char *word1 = (char *) malloc(200);
	char *mean = (char *) malloc(MAX_LEN);
	int rsize;
	int i=0;
	GtkListStore *store;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(mainapp->listview)));
	btpos(mainapp->btree,1);
	char *word2 = (char *) malloc(200);
	if (strcmp(word,"") == 0)
	{
		gtk_list_store_clear(store);
		btpos(mainapp->btree,1);
		i=0;
		while (btseln(mainapp->btree,word1,mean, MAX_LEN, &rsize) == 0)
		{
			if(strcmp(word1,"") >=0)
			{
				if (i==50)
					break;
				add_to_list(mainapp->listview,word1);
				i++;
			}
		}
		free(word1);
		free(mean);
		return;
	}
	strcpy(word2,word);
	char *s = check_character(mainapp->btree,word2);
	if (s!= NULL)
	{
		gtk_list_store_clear(store);
		i=0;
		add_to_list(mainapp->listview,s);
		while(btseln(mainapp->btree,word1,mean,MAX_LEN,&rsize) == 0)
		{
			if (strcmp(word1,word2)>=0)
			{
				if (i==50)
					break;
				add_to_list(mainapp->listview,word1);
				i++;
			}
		}
	}
	free(word1);
	free(mean);
}

void on_changed(GtkWidget *widget, gpointer data)
{
	App *mainapp = (App*) data;
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value = (char *) malloc(100);
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget),&model,&iter))
	{
		gtk_tree_model_get(model,&iter,0,&value, -1);
	}
	char *mean;
	const char *word = gtk_entry_get_text((GtkEntry*) mainapp->entry);
	word = strlwr((char*) word);
	mean = search_btree(mainapp->btree,value);
	int j;
	for (j=0;j<strlen(value);j++)
	{
		if (value[j]<=0)
			return;
	}
	gtk_text_buffer_set_text(mainapp->buffer,mean,-1);
}

void message(const char *text)
{
	GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);
	GtkWidget *dialog ;
	dialog = gtk_message_dialog_new((GtkWindow*) window, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,text);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void add_dialog(GtkWidget *widget, gpointer data)
{
	App *mainapp = (App *) data;
	GtkWidget *dialog;
	GtkWidget *word, *mean;
	GtkWidget *add, *cancel;
	GtkWidget *wordEntry, *meanText;
	dialog = gtk_dialog_new_with_buttons("Add new word",NULL,GTK_DIALOG_MODAL,GTK_STOCK_ADD,1,GTK_STOCK_CANCEL,2,NULL);
	gtk_window_set_title(GTK_WINDOW(dialog),"Add new word");
	gtk_window_set_default_size(GTK_WINDOW(dialog),500,300);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog),1);
	word = gtk_label_new("Word");
  	mean=gtk_label_new("Mean");
  	wordEntry = gtk_entry_new();
  	meanText = gtk_text_view_new();
  	gtk_text_view_set_editable(GTK_TEXT_VIEW(meanText),TRUE);
  	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(meanText),10);
  	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(meanText),10);
  	GtkTextBuffer *buffer;
  	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(meanText));
  	GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL,NULL);
  	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
  	gtk_container_add(GTK_CONTAINER(scrolledWindow),meanText);
  	GtkWidget *table;
  	table = gtk_table_new(5,5,FALSE);
  	gtk_table_attach_defaults(GTK_TABLE(table),word,0,1,0,1);
  	gtk_table_attach_defaults(GTK_TABLE(table),mean,0,1,1,4);
  	gtk_table_attach_defaults(GTK_TABLE(table),wordEntry,1,4,0,1);
  	gtk_table_attach_defaults(GTK_TABLE(table),scrolledWindow,1,4,1,4);
  	gtk_table_set_row_spacings(GTK_TABLE(table),0);
  	gtk_table_set_col_spacings(GTK_TABLE(table),0);
  	gtk_container_set_border_width(GTK_CONTAINER(meanText),1);
  	gtk_container_set_border_width(GTK_CONTAINER(scrolledWindow),1);
  	gtk_container_set_border_width(GTK_CONTAINER(table),2);
  	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
  	gtk_widget_show_all(dialog);
  	GtkTextIter begin, end;
  	gchar *text;
  	const char *word1;
  	int result = gtk_dialog_run(GTK_DIALOG(dialog));
  	while (result == 1)
  	{
  		gtk_text_buffer_get_bounds(buffer,&begin,&end);
  		text = gtk_text_buffer_get_text(buffer,&begin,&end,FALSE);
  		word1 = gtk_entry_get_text((GtkEntry *) wordEntry);
  		word1 = strlwr((char*) word1);
  		if (add_btree(mainapp->btree,(char*)word1,text))
  		{
  			message("Add new word successfully");
  			gtk_entry_set_text((GtkEntry*) wordEntry,"");
  			gtk_text_buffer_set_text(buffer,"",-1);
  		}
  		else
  			message("Cannot add new word. It may be existing in the Dictionary.");
  		result = gtk_dialog_run(GTK_DIALOG(dialog));
  	}
  	gtk_widget_destroy(dialog);
}

void edit_dialog(GtkWidget *widget, gpointer data)
{
	App *mainapp = (App *) data;
	GtkWidget *dialog;
	GtkWidget *word, *mean;
	GtkWidget *add, *cancel;
	GtkWidget *wordEntry, *meanText;
	dialog = gtk_dialog_new_with_buttons("Edit word",NULL,GTK_DIALOG_MODAL,"Show mean",1,GTK_STOCK_OK,2,GTK_STOCK_CANCEL,3,NULL);
	gtk_window_set_title(GTK_WINDOW(dialog),"Edit word");
	gtk_window_set_default_size(GTK_WINDOW(dialog),500,300);
	word = gtk_label_new("Word");
  	mean=gtk_label_new("Mean");
  	wordEntry = gtk_entry_new();
  	meanText = gtk_text_view_new();
  	gtk_text_view_set_editable(GTK_TEXT_VIEW(meanText),TRUE);
  	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(meanText),10);
  	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(meanText),10);
  	GtkTextBuffer *buffer;
  	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(meanText));
  	GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL,NULL);
  	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
  	gtk_container_add(GTK_CONTAINER(scrolledWindow),meanText);
  	GtkWidget *table;
  	table = gtk_table_new(1,1,TRUE);
  	gtk_table_attach_defaults(GTK_TABLE(table),word,0,1,0,1);
  	gtk_table_attach_defaults(GTK_TABLE(table),mean,0,1,1,4);
  	gtk_table_attach_defaults(GTK_TABLE(table),wordEntry,1,4,0,1);
  	gtk_table_attach_defaults(GTK_TABLE(table),scrolledWindow,1,4,1,4);
  	gtk_table_set_row_spacings(GTK_TABLE(table),0);
  	gtk_table_set_col_spacings(GTK_TABLE(table),0);
  	gtk_container_set_border_width(GTK_CONTAINER(meanText),1);
  	gtk_container_set_border_width(GTK_CONTAINER(scrolledWindow),1);
  	gtk_container_set_border_width(GTK_CONTAINER(table),2);
  	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
  	gtk_widget_show_all(dialog);
  	GtkTextIter begin, end;
  	gchar *text;
  	const char *word1;
  	char *find;
  	int result = gtk_dialog_run(GTK_DIALOG(dialog));
  	while (result == 1 || result == 2)
  	{
  		if(result == 1)
  		{
  			word1 = gtk_entry_get_text((GtkEntry *) wordEntry);
  			find = search_btree(mainapp->btree,(char*) word1);
  			if (find == NULL)
  				gtk_text_buffer_set_text(buffer,"Word does not exist\n",-1);
  			else
  				gtk_text_buffer_set_text(buffer,find,-1);
  		}
  		if (result == 2)
  		{
	  		gtk_text_buffer_get_bounds(buffer,&begin,&end);
	  		text = gtk_text_buffer_get_text(buffer,&begin,&end,FALSE);
	   		word1 = gtk_entry_get_text((GtkEntry *) wordEntry);
  			word1 = strlwr((char*) word1);
  			if (strcmp(word1,"") == 0 || strcmp(text,"") == 0)
  				message("Input a word. Then click \"Show mean\" to see its old mean. Click \"OK\" to confirm your changing.");
  			else
  			{
  				if(update_key(mainapp->btree,(char*) word1, text))
  				{
  					message("Edit successfully");
  					gtk_entry_set_text((GtkEntry*) wordEntry,"");
  					gtk_text_buffer_set_text(buffer,"",-1);
  				}
  				else
  					message ("Cannot edit");
  			}
  		}
  		result = gtk_dialog_run(GTK_DIALOG(dialog));
  	}
  	gtk_widget_destroy(dialog);
}

int delete_message(void)
{
	GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);
	GtkWidget *dialog = gtk_message_dialog_new((GtkWindow*) window, GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Do you want to delete this word?");
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	return result; //yes: -8, no: -9
}

void delete_dialog(GtkWidget *widget, gpointer data)
{
	App *mainapp = (App *) data;
	GtkWidget *dialog;
	GtkWidget *wordEntry, *label;
	dialog = gtk_dialog_new_with_buttons("Delete word",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,1,GTK_STOCK_CANCEL,2,NULL);
	gtk_window_set_title(GTK_WINDOW(dialog),"Delete word");
	label = gtk_label_new("Word");
	wordEntry = gtk_entry_new_with_max_length(200);
	gtk_window_set_default_size(GTK_WINDOW(dialog),350,150);
	GtkWidget *table;
	table = gtk_table_new(1,1,TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,0,2);
	gtk_table_attach_defaults(GTK_TABLE(table),wordEntry,0,1,3,5);
	gtk_table_set_row_spacings(GTK_TABLE(table),1);
	gtk_table_set_col_spacings(GTK_TABLE(table),1);
	gtk_container_set_border_width(GTK_CONTAINER(table),10);
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
	gtk_widget_show_all(dialog);
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	const char *word;
	char *find;
	while (result == 1)
	{
		word = gtk_entry_get_text((GtkEntry*) wordEntry);
		word = strlwr((char*) word);
		find = search_btree(mainapp->btree,(char*) word);
		if (find == NULL)
			message("This word does not exist in Dictionary");
		else
		{
			if (delete_message() == -8)
			{
				if (delete_key(mainapp->btree,(char *) word))
				{
					message("Delete successfully");
					gtk_entry_set_text((GtkEntry*) wordEntry,"");
				}
				else
					message("Cannot delete word");
			}
		}
		result = gtk_dialog_run(GTK_DIALOG(dialog));
	}
	gtk_widget_destroy(dialog);
}

void help_dialog(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog;
	dialog = gtk_dialog_new_with_buttons("Help",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,1,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),500,400);
	GtkWidget *meanText = gtk_text_view_new();
	gtk_window_set_title(GTK_WINDOW(dialog),"Help");
	gtk_text_view_set_editable(GTK_TEXT_VIEW(meanText),FALSE);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(meanText),10);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(meanText),10);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(meanText), GTK_WRAP_WORD);
	GtkTextBuffer *buffer;
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(meanText));
	GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
	gtk_container_add(GTK_CONTAINER(scrolledWindow),meanText);
	GtkWidget *table;
	table = gtk_table_new(1,1,TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table),scrolledWindow,0,4,0,4);
	gtk_table_set_row_spacings(GTK_TABLE(table),1);
	gtk_table_set_col_spacings(GTK_TABLE(table),1);
	gtk_container_set_border_width(GTK_CONTAINER(meanText),1);
	gtk_container_set_border_width(GTK_CONTAINER(scrolledWindow),1);
	gtk_container_set_border_width(GTK_CONTAINER(table),2);
	
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
	gtk_text_buffer_set_text(buffer,"Hướng dẫn sử dụng từ điển: \n\n1.Tìm kiếm: Gõ từ cần tìm vào ô tìm kiếm. Sau đó nhần nút Find để tìm nghĩa của từ\n\n2.Thêm từ: Trên cửa sổ chính nhấn nút Add sẽ hiện ra một hộp thoại. Nhập từ cần thêm vào ô Word và nghĩa của từ vào ô Mean. Sau đó nhấn Add để thêm từ.\n\n3. Chỉnh sửa từ: Trên cửa sổ chính, nhấn nút Edit sẽ hiện ra một hộp thoại. Nhập từ cần chỉnh sửa vào ô Word, nhấn Edit bắt đầu chỉnh sửa. Nhấn OK để hoàn tất. \n\n4.Xóa từ: Trên cửa sổ chính. Nhấn nút Delete để sẽ hiện ra hộp thoại xóa từ. Nhập từ cần xóa và nhấn OK để thực hiện.\n",-1);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

static gboolean auto_complete(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	App *mainapp = (App*) data;
	const char *word;
	if ((event->keyval) != 65289)
		return FALSE;
	btpos(mainapp->btree,1);
	int length = strlen((char*) word);
	char *word1 = (char *) malloc (200);
	char *mean = (char *) malloc (MAX_LEN);
	int rsize;
	int i=0;
	word = gtk_entry_get_text((GtkEntry *) mainapp->entry);
	word = strlwr((char *) word);
	while (btseln(mainapp->btree,(char*) word1, mean, MAX_LEN,&rsize) == 0)
	{
		if (strncmp(word1,word,strlen(word)) == 0)
			break;
		if (strncmp(word1,word,strlen(word)) > 0)
		{
			gtk_entry_set_text((GtkEntry*) (mainapp->entry),word);
			return TRUE;
		}
	}
	gtk_entry_set_text((GtkEntry*)(mainapp->entry),word1);
	free(word1);
	free(mean);
	return TRUE;
}

/*void show_suggest(GtkWidget *widget, gpointer data)
{
	App *mainapp = (App*) data;
	char *find, *find_soundex, *ptr;
	const char *word2 = gtk_entry_get_text((GtkEntry*) mainapp->entry);
	word2 = strlwr((char*) word2);
	char *word = strdup(word2);
	char *result, *result2, *mean2;
	result2 = (char*) malloc (MAX_LEN);
	BTA *btsoundex = btopn("Data/Anhvietsoundex.dict",0,0);
	GtkListStore *store;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(mainapp->listview)));
	int i=0;

	do
	{
		if (i==0)
			gtk_list_store_clear(store);
		i++;
		// tim tu co cung ma soundex voi word
		find_soundex = search_btree(btsoundex,soundex(word)); 
		if (find_soundex != NULL)
		{
			// tim tu o vi tri tiep theo trong soundex btree
			result = next_position_in_soundex_code(word,find_soundex); 
			if (result!=NULL)
			{
				// Them result vao list view
				add_to_list(mainapp->listview,result);
			}
		}
	}
	while (find_soundex != NULL || result != NULL);
	btcls(btsoundex);
}*/