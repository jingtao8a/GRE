./build/microbench \
--keys_file=/home/jingtao8a/GRE/datasets/books_200M_uint32 \
--keys_file_type=binary \
--read=0.5 --insert=0.5 \
--operations_num=800000000 \
--table_size=-1 \
--init_table_ratio=0.5 \
--thread_num=1 \
--index=DILI \
--memory
