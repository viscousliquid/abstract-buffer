#ifndef ABSTRACT_BUFFER_H
#define ABSTRACT_BUFFER_H

struct buffer_block {
   struct buffer_block *next;
};

struct buffer {
   int			        block_size;
   int    		        total_size;
   short int		    head_offset;
   short int		    tail_offset;
   struct buffer_block	*head;
   struct buffer_block	*tail;
};

struct buffer *new_buffer(int size, int count);
void free_buffer(struct buffer *buf);
int buffer_append(struct buffer *buf, char *data, int offset, int size);
int buffer_read(struct buffer *buf, char *dest, int offset, int size);
void flush_buffer(struct buffer *buf);

#endif
