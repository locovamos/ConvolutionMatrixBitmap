#include <gtk/gtk.h>
#include <string.h>
//gcc newgtk.c -o newgtk `pkg-config gtk+-3.0 --cflags pkg-config gtk+-3.0 --libs`
#define MAX_VALUE 255
static int customed_kernel_actived;
static int blur_kernel_actived;
static int edge_detection_kernel_actived;
static int sepia_actived;
static int grayscale_actived;
static char *sai_filename=" ";
static GtkFileChooser* file_button;
static GtkWidget* window;
static GtkToolItem *openfile1;
static FILE *fOut;
static GtkWidget* number1;
static GtkWidget* number2;
static GtkWidget* number3;
static GtkWidget* number4;
static GtkWidget* number5;
static GtkWidget* number6;
static GtkWidget* number7;
static GtkWidget* number8;
static GtkWidget* number9;
static GtkButton *add_the_filter_kernel_process;
static GtkWidget *support;
static GtkWidget *our_website;
static GtkWidget *quit;
GtkToggleButton *customed;
GtkToggleButton *blur_m;
GtkToggleButton *ridge_m;
GtkToggleButton *sepia_m;
GtkToggleButton *grayscale_m;
GtkWidget *window4;
GtkWidget *window3;


void load_press_callback(GtkWidget* event_box, GdkEventButton* event, gpointer data)
{
    GtkWidget* dialog;
    dialog = gtk_file_chooser_dialog_new("Open File",
        (GtkWindow*)window,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        ("_Cancel"),
        GTK_RESPONSE_CANCEL,
        ("_Open"),
        GTK_RESPONSE_ACCEPT,
        NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        sai_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    } else {
        
        sai_filename = " ";
    }
    gtk_widget_destroy(dialog);
}

void load_press_callback_error(GtkWidget* event_box, GdkEventButton* event, gpointer data)
{
    GtkWidget* dialog;
    dialog = gtk_file_chooser_dialog_new("Open File",
        (GtkWindow*)window,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        ("_Cancel"),
        GTK_RESPONSE_CANCEL,
        ("_Open"),
        GTK_RESPONSE_ACCEPT,
        NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        sai_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    } else {
        sai_filename = "";
    }
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(window4);
}

void ok_callback(GtkWidget* event_box, GdkEventButton* event, gpointer data)
{
    gtk_widget_destroy(window3);
}

void make_window_error_file()
{
    
    window4 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window4),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window4),30,30);
    gtk_window_set_title(GTK_WINDOW(window4),"Error");
    GtkWidget *grid4= gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window4),grid4);
    GtkWidget *warning=gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(warning),"<b>Error : You forgot to choose a file  </b>");
    gtk_grid_attach(GTK_GRID(grid4),warning,1,0,1,1);
    GtkWidget *choosewarning=gtk_button_new_with_label("Choose File");
    gtk_grid_attach(GTK_GRID(grid4),choosewarning,1,1,1,1);
    GdkPixbuf *imageres = gdk_pixbuf_new_from_file("erroricon.png",NULL);
    GtkWidget *imageres1 =gtk_image_new_from_pixbuf(imageres);
    gtk_grid_attach(GTK_GRID(grid4),imageres1,0,0,1,1);
    g_signal_connect(choosewarning, "clicked", G_CALLBACK(load_press_callback_error), NULL);
    gtk_widget_show_all(window4);
    
}

void make_window_error_choose()
{
    window3 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window3),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window3),30,30);
    gtk_window_set_title(GTK_WINDOW(window3),"Error");
    GtkWidget *grid3= gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window3),grid3);
    GtkWidget *warning=gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(warning),"<b>Error : Please select an option  </b>");
    gtk_grid_attach(GTK_GRID(grid3),warning,1,0,1,1);
    GdkPixbuf *imageres = gdk_pixbuf_new_from_file("erroricon.png",NULL);
    GtkWidget *imageres1 =gtk_image_new_from_pixbuf(imageres);
    gtk_grid_attach(GTK_GRID(grid3),imageres1,0,0,1,1);
    GtkWidget *choosewarning=gtk_button_new_with_label("Ok");
    gtk_grid_attach(GTK_GRID(grid3),choosewarning,1,1,1,1);
    g_signal_connect(choosewarning, "clicked", G_CALLBACK(ok_callback), NULL);
    gtk_widget_show_all(window3);
    
}
void image_grayscale()
{
    FILE *fIn = fopen(sai_filename,"r"); //Input File name
    FILE *fOut = fopen("test.bmp","w+"); //Output File name
    int i,y;
    unsigned char byte[54];
    if(fIn==NULL)
    {
        make_window_error_file();
        return;
    }
    for(i=0;i<54;i++) //read the 54 byte
    {
        byte[i] = getc(fIn);
    }
    fwrite(byte,sizeof(unsigned char),54,fOut);    //write the header back
    // extract image height, width and bitDepth from imageHeader
    int width = *(int*)&byte[18];
    int height = *(int*)&byte[22];
    //int bitDepth = *(int*)&byte[28];
    int size = height*width; //image size
    unsigned char buffer[size][3]; //to store the image data
    for(i=0;i<size;i++)    //RGB to gray
    {
        y=0;
        buffer[i][2]=getc(fIn);    //blue
        buffer[i][1]=getc(fIn); //green
        buffer[i][0]=getc(fIn);    //red
        y=(buffer[i][0]*0.3) + (buffer[i][1]*0.59) +(buffer[i][2]*0.11);
        //conversion formula of rgb to gray
        putc(y,fOut);
        putc(y,fOut);
        putc(y,fOut);
    }
    fclose(fOut);
    fclose(fIn);
}
void image_ridge()
{
    FILE *fIn = fopen(sai_filename,"r"); // Input File name
    FILE *fOut = fopen("test.bmp","w+"); // Output File name
    int i,j,y, x;
    unsigned char byte[54];
    if(fIn==NULL)
    {
        make_window_error_file();
        return;
    }
    for(i=0;i<54;i++) // read the 54 byte header from fIn
    {
        byte[i] = getc(fIn);
    }
    fwrite(byte,sizeof(unsigned char),54,fOut); // write the header back
    // extract image height, width and bitDepth from imageHeader
    int width = *(int*)&byte[18];
    int height = *(int*)&byte[22];
    //int bitDepth = *(int*)&byte[28];
    int size = height*width; // calculate the image size
    unsigned char buffer[size][3]; // store the input image data
    unsigned char out[size][3]; // store the output image data
    for(i=0;i<size;i++) // read image data character by character
    {
        buffer[i][2]=getc(fIn); // blue
        buffer[i][1]=getc(fIn); // green
        buffer[i][0]=getc(fIn); // red
    }
    float v=-1.0; // initialize the ridge kernel
    float kernel[3][3]={{v,v,v},
                        {v,8,v},
                        {v,v,v}};
    for(x=1;x<height-1;x++)
    {
        for(y=1;y<width-1;y++)
        {
            float sum0= 0.0;
            float sum1= 0.0;
            float sum2= 0.0;
            for(i=-1;i<=1;++i)
            {
                for(j=-1;j<=1;++j)
                {
                    // matrix multiplication with kernel with every color plane
                    sum0=sum0+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
                    sum1=sum1+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
                    sum2=sum2+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];
                }
            }
            out[(x)*width+(y)][0]=sum0;
            out[(x)*width+(y)][1]=sum1;
            out[(x)*width+(y)][2]=sum2;
        }
    }
    for(i=0;i<size;i++) //write image data back
    {
        putc(out[i][2],fOut);
        putc(out[i][1],fOut);
        putc(out[i][0],fOut);
    }
    fclose(fOut);
    fclose(fIn);
}
void image_sepia()
{
    FILE *fIn = fopen(sai_filename,"r"); //Input File name
    fOut = fopen("test.bmp","w+"); //Output File name
    int i,r,g,b;
    unsigned char byte[54];
    if(fIn==NULL) // check if the input file has not been opened succesfully.
    {
        make_window_error_file();
        return;
    }
    for(i=0;i<54;i++)  //read the 54 byte header from fIn
    {
        byte[i] = getc(fIn);
    }
    fwrite(byte,sizeof(unsigned char),54,fOut); //write the header back
    // extract image height, width and bitDepth from imageHeader
    int width = *(int*)&byte[18];
    int height = *(int*)&byte[22];
    //int bitDepth = *(int*)&byte[28];
    int size = height*width; //calculate image size
    unsigned char buffer[size][3]; //to store the image data
    for(i=0;i<size;i++)
    {
        r = 0;
        g = 0;
        b = 0;
        buffer[i][2] = getc(fIn); //blue
        buffer[i][1] = getc(fIn); //green
        buffer[i][0] = getc(fIn); //red
        //conversion formula of rgb to sepia
        r = (buffer[i][0]*0.393) + (buffer[i][1]*0.769)    + (buffer[i][2]*0.189);
        g = (buffer[i][0]*0.349) + (buffer[i][1]*0.686)    + (buffer[i][2]*0.168);
        b = (buffer[i][0]*0.272) + (buffer[i][1]*0.534)    + (buffer[i][2]*0.131);
        if(r > MAX_VALUE)
        { //if value exceeds
            r = MAX_VALUE;
        }
        if(g > MAX_VALUE){
            g = MAX_VALUE;
        }
        if(b > MAX_VALUE){
            b = MAX_VALUE;
        }
        putc(b,fOut);
        putc(g,fOut);
        putc(r,fOut);
    }
    fclose(fOut);
    fclose(fIn);
}
void image_blur()
{
    FILE *fIn = fopen(sai_filename,"r"); // Input File name
    fOut = fopen("test.bmp","w+"); // Output File name
    int i,j,y,x;
    unsigned char byte[54];
    if(fIn==NULL)
    {
        make_window_error_file();
        return;
    }
    for(i=0;i<54;i++) // read the 54 byte header from fIn
    {
        byte[i] = getc(fIn);
    }
    fwrite(byte,sizeof(unsigned char),54,fOut);    // write header back
    // extract image height, width and bitDepth from imageHeader
    int width = *(int*)&byte[18];
    int height = *(int*)&byte[22];
    //int bitDepth = *(int*)&byte[28];
    int size = height*width; //image size
    unsigned char buffer[size][3]; //input image data
    unsigned char out[size][3];     //output image data
    for(i=0;i<size;i++)    //read image data
    {
        buffer[i][2]=getc(fIn);    // blue
        buffer[i][1]=getc(fIn);    // green
        buffer[i][0]=getc(fIn); // red
    }
    float v=1.0 / 9.0; //blurrring kernel
    float kernel[3][3]={{v,v,v},
                        {v,v,v},
                        {v,v,v}};
    for(x=1;x<height-1;x++)
    {
        for(y=1;y<width-1;y++)
        {
            float sum0= 0.0;
            float sum1= 0.0;
            float sum2= 0.0;
            for(i=-1;i<=1;++i)
            {
                for(j=-1;j<=1;++j)
                {
                    // matrix multiplication with kernel with every color plane
                    sum0=sum0+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
                    sum1=sum1+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
                    sum2=sum2+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];
                }
            }
            out[(x)*width+(y)][0]=sum0;
            out[(x)*width+(y)][1]=sum1;
            out[(x)*width+(y)][2]=sum2;
        }
    }
    for(i=0;i<size;i++)    //write image data back to the file
    {
        putc(out[i][2],fOut);
        putc(out[i][1],fOut);
        putc(out[i][0],fOut);
    }
    fclose(fOut);
    fclose(fIn);
}
void image_matrix()
{
    FILE *fIn = fopen(sai_filename,"r"); // Input File name
    fOut = fopen("test.bmp","w+"); // Output File name
    int i,j,y, x;
    unsigned char byte[54];
    if(fIn==NULL)
    {
        make_window_error_file();
        return;
    }
    for(i=0;i<54;i++) // read the 54 byte header from fIn
    {
        byte[i] = getc(fIn);
    }
    fwrite(byte,sizeof(unsigned char),54,fOut); // write the header back
    // extract image height, width and bitDepth from imageHeader
    int width = *(int*)&byte[18];
    int height = *(int*)&byte[22];
    //int bitDepth = *(int*)&byte[28];
    int size = height*width; // calculate the image size
    unsigned char buffer[size][3]; // store the input image data
    unsigned char out[size][3]; // store the output image data
    for(i=0;i<size;i++) // read image data character by character
    {
        buffer[i][2]=getc(fIn); // blue
        buffer[i][1]=getc(fIn); // green
        buffer[i][0]=getc(fIn); // red
    }
    float num1 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    float num2 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number2)));
    float num3 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number3)));
    float num4 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number4)));
    float num5 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number5)));
    float num6 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number6)));
    float num7 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number7)));
    float num8 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number8)));
    float num9 = atof((char *)gtk_entry_get_text(GTK_ENTRY(number9)));
    float kernel[3][3]={{num1,num2,num3},
                        {num4,num5,num6},
                        {num7,num8,num9}};
    for(x=1;x<height-1;x++)
    {
        for(y=1;y<width-1;y++)
        {
            float sum0= 0.0;
            float sum1= 0.0;
            float sum2= 0.0;
            for(i=-1;i<=1;++i)
            {
                for(j=-1;j<=1;++j)
                {
                    // matrix multiplication with kernel with every color plane
                    sum0=sum0+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
                    sum1=sum1+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
                    sum2=sum2+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];
                }
            }
            out[(x)*width+(y)][0]=sum0;
            out[(x)*width+(y)][1]=sum1;
            out[(x)*width+(y)][2]=sum2;
        }
    }
    for(i=0;i<size;i++) //write image data back
    {
        putc(out[i][2],fOut);
        putc(out[i][1],fOut);
        putc(out[i][0],fOut);
    }
    fclose(fOut);
    fclose(fIn);
    
}
static gboolean close_window(GtkWidget* event_box, GdkEventButton* event, gpointer data)
{
    gtk_main_quit();
    remove("test.bmp");
    return TRUE;
}


void make_window_image()
{
    GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window2),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window2),300,300);
    gtk_window_set_title(GTK_WINDOW(window2),"Image processed");
    GtkWidget *grid2= gtk_grid_new();
    GdkPixbuf *imageres = gdk_pixbuf_new_from_file("test.bmp",NULL);
    GdkPixbuf *imageress = gdk_pixbuf_new_from_file(sai_filename,NULL);
    GtkWidget *imageres1 =gtk_image_new_from_pixbuf(imageres);
    GtkWidget *imageress1 =gtk_image_new_from_pixbuf(imageress);
    GtkWidget *before=gtk_label_new(NULL);
    gtk_label_set_markup (GTK_LABEL (before), "<b>BEFORE</b>");
    GtkWidget *after=gtk_label_new(NULL);
    gtk_label_set_markup (GTK_LABEL (after), "<b>AFTER</b>");
    gtk_container_add(GTK_CONTAINER(window2),grid2);
    gtk_grid_attach(GTK_GRID(grid2),after,1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid2),before,0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid2),imageress1,0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid2),imageres1,1,1,1,1);
    gtk_widget_show_all(window2);
}
void load_image_new(GtkWidget* event_box, GdkEventButton* event, gpointer data)
{
    
    FILE *fnew=fopen(sai_filename,"r");
    if(customed_kernel_actived == 1)
    {
        if(fnew==NULL)
        {
            image_matrix();
            return;
        }
        image_matrix();
        make_window_image();
        gtk_toggle_button_set_active(customed,FALSE);
        return;
    }
    if(blur_kernel_actived == 1)
    {
        if(fnew==NULL)
        {
            image_blur();
            return;
        }
        image_blur();
        make_window_image();
        gtk_toggle_button_set_active(blur_m,FALSE);
        return;
    }
    if(edge_detection_kernel_actived == 1)
    {
        if(fnew==NULL)
        {
            image_ridge();
            return;
        }
        image_ridge();
        make_window_image();
        gtk_toggle_button_set_active(ridge_m,FALSE);
        return;
    }
    if(sepia_actived == 1)
    {
        if(fnew==NULL)
        {
            image_sepia();
            return;
        }
        image_sepia();
        make_window_image();
        gtk_toggle_button_set_active(sepia_m,FALSE);
        return;
    }
    if(grayscale_actived == 1)
    {
        if(fnew==NULL)
        {
            image_grayscale();
            return;
        }
        image_grayscale();
        make_window_image();
        gtk_toggle_button_set_active(grayscale_m,FALSE);
        return;
    }
    make_window_error_choose();

    
}
void get_active_customed_kernel (GtkToggleButton *source, gpointer data)
{
    customed_kernel_actived = gtk_toggle_button_get_active (source);
}
void get_active_blur_kernel (GtkToggleButton *source, gpointer data) {
    blur_kernel_actived = gtk_toggle_button_get_active (source);
}
void get_active_edge_detection_kernel (GtkToggleButton *source, gpointer data) {
    edge_detection_kernel_actived = gtk_toggle_button_get_active (source);
}
void get_active_sepia (GtkToggleButton *source, gpointer data) {
    sepia_actived = gtk_toggle_button_get_active (source);
}
void get_grayscale (GtkToggleButton *source, gpointer data) {
    grayscale_actived = gtk_toggle_button_get_active (source);
}
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "gtk.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    window = GTK_WIDGET(gtk_builder_get_object(builder, "org.gtk.ocr"));
    openfile1 = GTK_TOOL_ITEM(gtk_builder_get_object(builder, "file_button"));
    number1 = GTK_WIDGET(gtk_builder_get_object(builder, "number1"));
    number2 = GTK_WIDGET(gtk_builder_get_object(builder, "number2"));
    number3 = GTK_WIDGET(gtk_builder_get_object(builder, "number3"));
    number4 = GTK_WIDGET(gtk_builder_get_object(builder, "number4"));
    number5 = GTK_WIDGET(gtk_builder_get_object(builder, "number5"));
    number6 = GTK_WIDGET(gtk_builder_get_object(builder, "number6"));
    number7 = GTK_WIDGET(gtk_builder_get_object(builder, "number7"));
    number8 = GTK_WIDGET(gtk_builder_get_object(builder, "number8"));
    number9 = GTK_WIDGET(gtk_builder_get_object(builder, "number9"));
    customed=GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "customed_kernel"));
    blur_m=GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "blur_kernel"));
    ridge_m=GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "edge_detection_kernel"));
    sepia_m=GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "sepia"));
    grayscale_m=GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "grayscale"));
    
    
    
    add_the_filter_kernel_process = GTK_BUTTON(gtk_builder_get_object(builder, "add_kernel"));
    customed_kernel_actived = 0,
    blur_kernel_actived = 0,
    edge_detection_kernel_actived = 0,
    sepia_actived = 0,
    grayscale_actived = 0,
    our_website = GTK_WIDGET(gtk_builder_get_object(builder, "our_website"));
    support = GTK_WIDGET(gtk_builder_get_object(builder, "support"));
    quit = GTK_WIDGET(gtk_builder_get_object(builder, "quit"));
    gtk_window_set_title(GTK_WINDOW(window),"Image Filtering");
    g_signal_connect (GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "customed_kernel")), "toggled", G_CALLBACK (get_active_customed_kernel), NULL);
    g_signal_connect (GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "blur_kernel")), "toggled", G_CALLBACK (get_active_blur_kernel), NULL);
    g_signal_connect (GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "edge_detection_kernel")), "toggled", G_CALLBACK (get_active_edge_detection_kernel), NULL);
    g_signal_connect (GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "sepia")), "toggled", G_CALLBACK (get_active_sepia), NULL);
    g_signal_connect (GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "grayscale")), "toggled", G_CALLBACK (get_grayscale), NULL);
    g_signal_connect(window,"destroy", G_CALLBACK(close_window), NULL);
    g_signal_connect(openfile1, "clicked", G_CALLBACK(load_press_callback), NULL);
    g_signal_connect(add_the_filter_kernel_process, "clicked", G_CALLBACK(load_image_new), NULL);
    g_signal_connect(quit, "clicked", G_CALLBACK(close_window), NULL);
    gtk_main();
    return 0;
}
