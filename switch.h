//switch.h


struct switch_table_format {
	int valid_status;	
	char dest;	
	int port_num;	
};

void switch_main(int switch_id);
