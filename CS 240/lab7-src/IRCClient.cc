#include <stdio.h>
#include <gtk/gtk.h>
#include <time.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

GtkListStore * list_rooms;
GtkListStore * list_users;
GtkListStore * list_messages;

char * host = "localhost";
char * user = (char *) malloc (20 * sizeof(char));
char * password = (char *) malloc (20 * sizeof(char));
char * message = (char *) malloc (300 * sizeof(char));
char * tmessage = (char *) malloc (200 * sizeof(char));
char * room = (char *) malloc (50 * sizeof(char));
char * sport = (char *) malloc (20 * sizeof(char));
char * args = (char *) malloc (300 * sizeof(char));
int port = 2038;
int roomCount = 0;
int log_user = 0;
int log_room = 0;
int log_messages = 0;
int timesdone = 0;
char *preval;

GtkWidget *tree_view;
GtkTreeSelection *selection;

#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 300
#define MAX_RESPONSE (20 * 1024)

int lastMessage = 0;
int timesRun = 0;


int open_client_socket(char * host, int port) {
	// Initialize socket address structure
	struct  sockaddr_in socketAddress;
	
	// Clear sockaddr structure
	memset((char *)&socketAddress, 0, sizeof(socketAddress));
	
	// Set family to Internet 
	socketAddress.sin_family = AF_INET;
	
	// Set port
	socketAddress.sin_port = htons((u_short)port);
	
	// Get host table entry for this host
	struct  hostent  *ptrh = gethostbyname(host);
	if ( ptrh == NULL ) {
		perror("gethostbyname");
		exit(1);
	}
	
	// Copy the host ip address to socket address structure
	memcpy(&socketAddress.sin_addr, ptrh->h_addr, ptrh->h_length);
	
	// Get TCP transport protocol entry
	struct  protoent *ptrp = getprotobyname("tcp");
	if ( ptrp == NULL ) {
		perror("getprotobyname");
		exit(1);
	}
	
	// Create a tcp socket
	int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}
	
	// Connect the socket to the specified server
	if (connect(sock, (struct sockaddr *)&socketAddress,
		    sizeof(socketAddress)) < 0) {
		perror("connect");
		exit(1);
	}
	
	return sock;
}

int sendCommand(char * host, int port, char * command, char * user,
		char * password, char * args, char * response) {
	int sock = open_client_socket( host, port);

	// Send command
	write(sock, command, strlen(command));
	write(sock, " ", 1);
	write(sock, user, strlen(user));
	write(sock, " ", 1);
	write(sock, password, strlen(password));
	write(sock, " ", 1);
	write(sock, args, strlen(args));
	write(sock, "\r\n",2);

	// Keep reading until connection is closed or MAX_REPONSE
	int n = 0;
	int len = 0;
	while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
		len += n;
	}

	//printf("response:%s\n", response);

	close(sock);
}


/*
void get_messages() {
	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "GET-MESSAGES", user, password, room, "", response);
	if (!strcmp(response,"OK\r\n")) {
		printf("Room %s created\n", room);
		roomCount++;
	}
}*/

void send_message() {
	char response[ MAX_RESPONSE ] = {0};
	response[0] = '\0';
	args = strdup(room);
	strcat(args, " ");
	strcat(args, message);
	sendCommand(host, port, "SEND-MESSAGE", user, password, args, response);
	if (!strcmp(response,"OK\r\n")) {
		//log_messages = 1;
		printf("Message %s sent\n", args);
	}
}
/*
void send_message_mod(char * argument) {
	char response[ MAX_RESPONSE ];
	response[0] = '\0';
	args = strdup(room);
	strcat(args, argument);
	sendCommand(host, port, "SEND-MESSAGE", user, password, args, response);
	if (!strcmp(response,"OK\r\n")) {
		//log_messages = 1;
		printf("Message %s sent\n", args);
	}
}*/

void leave_room(char * troom) {
	char response[ MAX_RESPONSE ] = {0};
	response[0] = '\0';
	strcpy(args, troom);
	//char * tt = (char *) malloc (100 * sizeof(char));
	sendCommand(host, port, "LEAVE-ROOM", user, password, args, response);
	if (!strcmp(response,"OK\r\n")) {
		//tt = strdup(user);
		//strcat(tt, " has left the room");
		//send_message_mod(tt);
		printf("User %s removed\n", user);
	}
}



void update_list_rooms() {
    GtkTreeIter iter;
    int i;
	char * temp = (char *) malloc (30 * sizeof(char));
	gchar *msg;
	//timesdone = 0;
    /* Add some messages to the window */
	if (log_user == 1) {
		char responserooms[ MAX_RESPONSE ] = {0};
		responserooms[0] = '\0';
		sendCommand(host, port, "LIST-ROOMS", user, password, "default", responserooms);
		printf("response: %s\n", responserooms);
		temp = strtok(responserooms, "\r\n");
		msg = g_strdup_printf ("%s", temp);
        gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        gtk_list_store_set (GTK_LIST_STORE (list_rooms), 
	                    &iter,
                            0, msg,
	                    -1);
		roomCount++;
		
		while ((temp = strtok(NULL, "\r\n")) != NULL) {

			msg = g_strdup_printf ("%s", temp);
			printf("ROOMS %s\n", msg);	        
			gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
	        gtk_list_store_set (GTK_LIST_STORE (list_rooms), 
	                    &iter,
                            0, msg,
	                    -1);
			roomCount++;
		}
		//g_free(msg);
	}
	/*if (log_user == 1) {
        gchar *msg = g_strdup_printf ("%s", room);
        gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        gtk_list_store_set (GTK_LIST_STORE (list_rooms), 
	                    &iter,
                            0, msg,
	                    -1);
	}*/
}

void create_room() {
	char response[ MAX_RESPONSE ] = {0};
	response[0] = '\0';
	sendCommand(host, port, "CREATE-ROOM", user, password, room, response);
	if (!strcmp(response,"NO\r\n")) {
		printf("Room couldn't be added cause you fool\n");
		log_room = 1;
		return;	
	} else if (!strcmp(response, "OK\r\n")) {
		log_room = 1;
		printf("Room %s added\n", room);
		//log_user = 1;
		//gtk_list_store_clear(GTK_LIST_STORE(list_rooms));
		//update_list_rooms();
	}
	//sendCommand(host, port, "LIST-ROOMS", user, password, "", "", response);
	//printf("%s\n", response);
}

void enter_room() {
	char response[ MAX_RESPONSE ] = {0};
	response[0] = '\0';
	//char * tt = (char *) malloc (100 * sizeof(char));
	sendCommand(host, port, "ENTER-ROOM", user, password, room, response);
	if (!strcmp(response, "OK\r\n")) {
		//tt = strdup(user);
		//strcat(tt, " has entered the room");
		//send_message_mod(tt);
		//sendCommand(host, port, "ENTER-ROOM", user, password, "User ", response);
		gtk_list_store_clear(GTK_LIST_STORE(list_rooms));
		strcat(message, " has joined room");
		send_message();
		gtk_list_store_clear(GTK_LIST_STORE(list_rooms));
		//send_message_room();
		update_list_rooms();
		printf("User %s added\n", user);
	}
}

/*void get_users_in_room(char * troom) {
	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "ENTER-ROOM", user, password, troom, "", response);
}*/

void update_list_users() {
    GtkTreeIter iterator;
    int i;
	char * temp = (char *) malloc (30 * sizeof(char));
	gchar *msg;
    //Add some messages to the window
	if (log_user == 1) {    	
		char responseuser[ MAX_RESPONSE ] = {0};
		responseuser[0] = '\0';
		if (room != NULL) {
			sendCommand(host, port, "GET-USERS-IN-ROOM", user, password, room, responseuser);
			temp = strdup(responseuser);
			temp = strtok(temp, "\r\n");
			msg = g_strdup_printf ("%s", temp);
        	gtk_list_store_append (GTK_LIST_STORE (list_users), &iterator);
        	gtk_list_store_set (GTK_LIST_STORE (list_users), 
	                  	 	 &iterator,
                      	      0, msg,
	                 	   -1);
			while ((temp = strtok(NULL, "\r\n")) != NULL) {
					msg = g_strdup_printf ("%s", temp);
	       			gtk_list_store_append (GTK_LIST_STORE (list_users), &iterator);
	        		gtk_list_store_set (GTK_LIST_STORE (list_users), 
	                    		&iterator,
                            	0, msg,
	                    		-1);
			}
		}
	//g_free(msg);
	}
}

void add_user() {
	// Try first to add user in case it does not exist.
	char responseadduser[ MAX_RESPONSE ] = {0};
	responseadduser[0] = '\0';
	sendCommand(host, port, "ADD-USER", user, password, "default", responseadduser);
	if (!strcmp(responseadduser, "OK\r\n")) {
		printf("User %s added\n", user);
		log_user = 1;
		//update_list_users();
		//update_list_rooms();
	} else if (!strcmp(responseadduser, "NO\r\n")) {
		printf("User %s logged in\n", user);
		log_user = 1;
		//update_list_users();
		//update_list_rooms();
	}
}

/* Create the list of "messages" */
static GtkWidget *create_list( const char * titleColumn, GtkListStore *model )
{
    GtkWidget *scrolled_window;
    //GtkListStore *model;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;

    int i;
   
    /* Create a new scrolled window, with scrollbars only if needed */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				    GTK_POLICY_AUTOMATIC, 
				    GTK_POLICY_AUTOMATIC);
   
    //model = gtk_list_store_new (1, G_TYPE_STRING);
    tree_view = gtk_tree_view_new ();
    gtk_container_add (GTK_CONTAINER (scrolled_window), tree_view);
    gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model));
    gtk_widget_show (tree_view);
   
    cell = gtk_cell_renderer_text_new ();

    column = gtk_tree_view_column_new_with_attributes (titleColumn,
                                                       cell,
                                                       "text", 0,
                                                       NULL);
  
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),
	  		         GTK_TREE_VIEW_COLUMN (column));

    return scrolled_window;
}
   
/* Add some text to our text widget - this is a callback that is invoked
when our window is realized. We could also force our window to be
realized with gtk_widget_realize, but it would have to be part of
a hierarchy first */

static void insert_text( GtkTextBuffer *buffer, const char * initialText )
{
   GtkTextIter iter;
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
   gtk_text_buffer_insert (buffer, &iter, initialText,-1);
}
   
/* Create a scrolled text area that displays a "message" */
static GtkWidget *create_text( const char * initialText )
{
   GtkWidget *scrolled_window;
   GtkWidget *view;
   GtkTextBuffer *buffer;

   view = gtk_text_view_new ();
   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
		   	           GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC);

   gtk_container_add (GTK_CONTAINER (scrolled_window), view);
   insert_text (buffer, initialText);

   gtk_widget_show_all (scrolled_window);

   return scrolled_window;
}

static void enteruser_callback(GtkWidget *entry) {
	const gchar *entry_text;
	entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
	user = strdup(entry_text);	
	printf("Entry contents: %s\n", entry_text);
}

static void enterpass_callback(GtkWidget *entry) {
	const gchar *entry_text;
	entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
	password = strdup(entry_text);
	add_user();
	printf("Entry contents: %s\n", entry_text);
}

static void enterroom_callback(GtkWidget *entry) {
	const gchar *entry_text;
	entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
	room = strdup(entry_text);	
	create_room();	
	printf("Entry contents: %s\n", entry_text);
}

static void on_changed(GtkWidget * widget, GtkWidget * Widgetone) {
	//get the new room name and leave user from previous room
	GtkTreeIter iter;
  	GtkTreeModel *model;
	char * value;
	//printf("Sefdf\n");
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
    	gtk_tree_model_get(model, &iter, 0, &value,  -1);
		printf("%s\n", value);
		room = strdup(value);
		if (preval == NULL) {
			printf("preval is same\n");
			preval = strdup(value);
			log_messages = 1;
			enter_room();
			//gtk_list_store_clear(GTK_LIST_STORE(list_users));
			//update_list_users();
		} else {
			printf("%s\n", preval);
			leave_room(preval);
			//strcat(message, " has left room");
			//_message();
			enter_room();
			log_messages = 1;
			gtk_list_store_clear(GTK_LIST_STORE(list_users));
			update_list_users();
		}
		preval = strdup(value);
    //gtk_label_set_text(GTK_LABEL(label), value);
    //g_free(value);
  	}
	//get new room through selection

	//get previous selection - call leaveuser on it and change the prev selection
}

static void update_list_messages() {
	GtkTreeIter iterators;
    int i;
	char * temp = (char *) malloc (300 * sizeof(char));
	gchar *msg;
    //Add some messages to the window
	if (log_messages == 1) {    
		char responsemessage[ MAX_RESPONSE ] = {0};
		responsemessage[0] = '\0';
		sendCommand(host, port, "GET-MESSAGES", user, password, room, responsemessage);
		temp = strdup(responsemessage);
		temp = strtok(temp, "\r\n");
		msg = g_strdup_printf ("%s", temp);
   		gtk_list_store_append (GTK_LIST_STORE (list_messages), &iterators);
    	gtk_list_store_set (GTK_LIST_STORE (list_messages), 
		                &iterators,
 	                       0, msg,
		                -1);
		while ((temp = strtok(NULL, "\r\n")) != NULL) {
			msg = g_strdup_printf ("%s", temp);
		    gtk_list_store_append (GTK_LIST_STORE (list_messages), &iterators);
		    gtk_list_store_set (GTK_LIST_STORE (list_messages), 
		                &iterators,
    	                    0, msg,
		                -1);
		}
		//g_free(msg);
	}
}

static void get_messages() {
	gtk_list_store_clear(GTK_LIST_STORE(list_messages));
	update_list_messages();
}

static void message_callback(GtkWidget *entry) {
	const gchar *entry_text;
	entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
	message = strdup(entry_text);
	log_messages = 1;
	send_message();
	printf("%s\n", room);
	//get_messages();
		
	printf("Entry contents: %s\n", entry_text);
}

static void initialize() {
	timesdone = 0;
	preval = NULL;
}

static gboolean
time_handler(GtkWidget *widget)
{
	gtk_list_store_clear(GTK_LIST_STORE(list_rooms));
	update_list_rooms();
	gtk_list_store_clear(GTK_LIST_STORE(list_users));
	update_list_users();
	get_messages();
	return TRUE;
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;
    GtkWidget *list;
    GtkWidget *messages;
    GtkWidget *myMessage;
    GtkWidget *msguser;
    GtkWidget *msgpass;
    GtkWidget *msgroom;
    GtkWidget *hbox, *vbox;
    GtkWidget *entryuser;
    GtkWidget *entrypass;
    GtkWidget *entryroom;
    GtkWidget *check;
    GtkWidget *sendmessage;
	GtkWidget * frameuser;
	GtkWidget * frameroom;
    gint tmp_pos;
	
    gtk_init (&argc, &argv);
   
	GdkColor color;
	gdk_color_parse ("#159AC1", &color);   
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "IRC Client");
    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
	g_signal_connect (window, "destroy",
	              G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_size_request (GTK_WIDGET (window), 950, 800);
		
	//initialize
	initialize();

    // Create a table to place the widgets. Use a 7x4 Grid (7 rows x 4 columns)
    GtkWidget *table = gtk_table_new (20, 6, TRUE);
    gtk_container_add (GTK_CONTAINER (window), table);
    gtk_table_set_row_spacings(GTK_TABLE (table), 6);
    gtk_table_set_col_spacings(GTK_TABLE (table), 6);
    gtk_widget_show (table);

    // Add list of rooms. Use columns 0 to 4 (exclusive) and rows 0 to 4 (exclusive)
    list_rooms = gtk_list_store_new (1, G_TYPE_STRING);
    //update_list_rooms();
    list = create_list ("Rooms", list_rooms);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 2, 4, 0, 9);
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  	g_signal_connect(tree_view, "row-activated", 
      G_CALLBACK(on_changed), selection);
    gtk_widget_show (list);
	
	// Add list of users. Use columns 0 to 4 and rows 0 to 4
    list_users = gtk_list_store_new (1, G_TYPE_STRING);
    //update_list_users();
    list = create_list ("Users", list_users);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 4, 6, 0, 9);
    gtk_widget_show (list);

	// Add send button. Use columns 0 to 1 (exclusive) and rows 4 to 7 (exclusive)
	GtkWidget *send_button = gtk_button_new_with_label ("Send");
    gtk_table_attach_defaults(GTK_TABLE (table), send_button, 2, 4, 18, 20); 
	//gtk_widget_set_size_request(send_button, 80, 33);
	//g_signal_connect_swapped (send_button, "clicked",
	//		      G_CALLBACK (get_messages),
    //                          window);
	gtk_widget_show (send_button);
   
    // Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive)
    list_messages = gtk_list_store_new (1, G_TYPE_STRING);
    //update_list_rooms();
    list = create_list ("Messages", list_messages);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 0, 6, 10, 16);
	//selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  	//g_signal_connect_swapped(send_button, "clicked", 
    //  G_CALLBACK(get_messages), selection);
    gtk_widget_show (list);
    // Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive)
	
	//button
	GtkWidget *adduser_button = gtk_button_new_with_label ("Create Account");
    gtk_table_attach_defaults(GTK_TABLE (table), adduser_button, 0, 2, 3, 4);   
	gtk_widget_show (adduser_button);

	GtkWidget *addroom_button = gtk_button_new_with_label ("Create Room");
    gtk_table_attach_defaults(GTK_TABLE (table), addroom_button, 0, 2, 8, 9);
	gtk_widget_set_size_request(addroom_button, 80, 35); 
    gtk_widget_show (addroom_button);

	GtkWidget *login_button = gtk_button_new_with_label ("Login");
    gtk_table_attach_defaults(GTK_TABLE (table), login_button, 0, 2, 4, 5);
    gtk_widget_show (login_button);

	GtkWidget *createroom_button = gtk_button_new_with_label ("Create Room");
    gtk_table_attach_defaults(GTK_TABLE (table), createroom_button, 0, 2, 8, 9); 
    //gtk_widget_show (createroom_button);

	//username
	entryuser = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (entryuser), 50);
    g_signal_connect_swapped (adduser_button, "clicked",
		      G_CALLBACK (enteruser_callback),
		      entryuser);
	g_signal_connect_swapped (login_button, "clicked",
		      G_CALLBACK (enteruser_callback),
		      entryuser);
    //gtk_entry_set_text (GTK_ENTRY (entryuser), "Username");
    tmp_pos = GTK_ENTRY (entryuser)->text_length;
	gtk_table_attach_defaults(GTK_TABLE (table), entryuser, 0, 2, 1, 2);
	gtk_widget_set_size_request(entryuser, 80, 30);
	gtk_widget_show (entryuser);

	//frames
	frameuser = gtk_frame_new("USER INFO");
	//gtk_frame_set_label_widget(frame, entryuser);
	gtk_table_attach_defaults(GTK_TABLE (table), frameuser, 0, 2, 0, 5);
	gtk_widget_show(frameuser);

	frameroom = gtk_frame_new("ROOM INFO");
	//gtk_frame_set_label_widget(frame, entryuser);
	gtk_table_attach_defaults(GTK_TABLE (table), frameroom, 0, 2, 6, 9);
	gtk_widget_show(frameroom);

	//password
	entrypass = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (entrypass), 50);
    g_signal_connect_swapped (adduser_button, "clicked",
		      G_CALLBACK (enterpass_callback),
		      entrypass);
	g_signal_connect_swapped (login_button, "clicked",
		      G_CALLBACK (enterpass_callback),
		      entrypass);
    //gtk_entry_set_text (GTK_ENTRY (entrypass), "password");
	gtk_entry_set_visibility (GTK_ENTRY (entrypass), FALSE);    
	tmp_pos = GTK_ENTRY (entrypass)->text_length;	
	gtk_table_attach_defaults(GTK_TABLE (table), entrypass, 0, 2, 2, 3);
	gtk_widget_set_size_request(entrypass, 80, 30);
	gtk_widget_show (entrypass);


	//room name
	entryroom = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (entryroom), 50);
	g_signal_connect_swapped (addroom_button, "clicked",
		      G_CALLBACK (enterroom_callback),
		      entryroom);
	g_signal_connect_swapped (createroom_button, "clicked",
		      G_CALLBACK (enterroom_callback),
		      entryroom);
    //gtk_entry_set_text (GTK_ENTRY (entryroom), "RoomName");
    tmp_pos = GTK_ENTRY (entryroom)->text_length;
	gtk_table_attach_defaults(GTK_TABLE (table), entryroom, 0, 2, 7, 8);
	gtk_widget_set_size_request(entryroom, 80, 32);
	gtk_widget_show (entryroom);

	//Send Message

	sendmessage = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (sendmessage), 300);
    g_signal_connect_swapped (send_button, "clicked",
		      G_CALLBACK (message_callback),
		      sendmessage);
	gtk_widget_set_size_request(sendmessage, 80, 50);
    //gtk_entry_set_text (GTK_ENTRY (sendmessage), "Send message");
    tmp_pos = GTK_ENTRY (sendmessage)->text_length;
	gtk_table_attach_defaults(GTK_TABLE (table), sendmessage, 0, 6, 16, 18);
	gtk_widget_show (sendmessage);	

    gtk_widget_show (table);
    gtk_widget_show (window);

	g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) window);
	time_handler(window);

    gtk_main ();

    return 0;
}



