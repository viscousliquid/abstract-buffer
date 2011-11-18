#include "abstract-buffer.h"

int allocate_block(struct buffer *buf)
{
	struct buffer_block *block = malloc(buf->block_size + 1);

	if (block) {
		if (buf->tail) {
			block->next = buf->tail->next;
			buf->tail->next = block;
			buf->block_count++;
		} else {
			if (buf->head) {
				buf->head->next = block;
				block->next = buf->head;
			} else
				buf->head = block;
		}
		return 0;
	} else
		return ENOMEM;
}

struct buffer *new_buffer(int size, int count)
{
	struct buffer *buf = malloc(sizeof(struct buffer));

	memset(buf, 0, sizeof(struct buffer));
	buf->block_size = size;

	while (count) {
		if (allocate_block(buf) != 0) {
			free_buffer(buf);
			*buf = NULL;
			break;
		}
		count--;
	}

	return buf;
}

void free_buffer(struct buffer *buf);
{
	struct buffer_block *iter, *next;

	itr = buf->head;
	do {
		next = itr->next;
		free(itr);
	} while (next != buf->head);

	free(buf);
}

int append_data(struct buffer *buf, char *data, int offset, int size);
{
	int req_blocks = (int) ceil((double) size / buf->block_size);

	while ((buf->count - buf->utilized) < req_blocks)
		if (allocate_block(buf) != 0)
			return -1;
}

int read_data(struct buffer *buf, char *dest, int offset, int size);
{
}

void flush(struct buffer *buf);
{
}
