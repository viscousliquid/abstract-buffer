#ifndef ABSTRACT_BUFFER_H
#define ABSTRACT_BUFFER_H

struct buffer_block {
   struct buffer_block *next;
};

struct buffer {
   int			block_size;
   int			block_count;
   int			free;
   short int		head_offset;
   short int		tail_offset;
   struct buffer_block	*head;
   struct buffer_block	*tail;
};

struct buffer *new_buffer(int size, int count);
void free_buffer(struct buffer *buf);
int append_data(struct buffer *buf, char *data, int offset, int size);
int read_data(struct buffer *buf, char *dest, int offset, int size);
void flush(struct buffer *buf);

#endif
