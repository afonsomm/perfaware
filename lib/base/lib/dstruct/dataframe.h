

// TODO
#ifndef BASE_DSTRUCT_DATAFRAME_H
# define BASE_DSTRUCT_DATAFRAME_H


typedef struct DataFrameColumn DataFrameColumn;
struct DataFrameColumn {
	HashMapEntry hashmap_entry;
	Array data;
	String8 name;
};


typedef struct DataFrame DataFrame;
struct DataFrame {
	HashMapSlot* columns;
	u32 n_rows;
	u32 n_cols;
};


internalf DataFrame* dataframe_alloc(String8List* column_names, u32 n_rows, u32 n_cols);
internalf void dataframe_add_column(String8 name);


#endif  // BASE_DSTRUCT_DATAFRAME_H
