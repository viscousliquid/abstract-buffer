#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "abstract-buffer.h"

int allocate_block(struct buffer *buf)
{
    int size = buf->block_size + sizeof(struct buffer_block);
	struct buffer_block *block = malloc(size);
    memset(block, 0, size);

	if (block) {
		if (buf->tail) {
			buf->tail->next = block;
            buf->tail = block;
            buf->tail_offset = 0;
		} else {
            buf->head = buf->tail = block;
            buf->head_offset = 0;
            buf->tail_offset = 0;
		}
		buf->total_size += buf->block_size;
		return 0;
	} else
		return ENOMEM;
}

void rollback_append(struct buffer *buf, struct buffer_block *block, int offset)
{
    struct buffer_block *next, *itr = block->next;

    while (itr) {
        next = itr->next;
        free(itr);
        itr = next;
    }

    buf->tail = block;
    buf->tail_offset = offset;
}

struct buffer *new_buffer(int block_size)
{
	struct buffer *buf = malloc(sizeof(struct buffer));

	memset(buf, 0, sizeof(struct buffer));
	buf->block_size = block_size;

	if (allocate_block(buf) != 0) {
		free_buffer(buf);
		buf = NULL;
	}

	return buf;
}

void free_buffer(struct buffer *buf)
{
	struct buffer_block *iter, *next;

	iter = buf->head;
	while (iter) {
		next = iter->next;
		free(iter);
        iter = next;
	}

	free(buf);
}

int buffer_append(struct buffer *buf, char *src, int offset, int size)
{
    struct buffer_block *cp_block = buf->tail;
    int cp_offset = buf->tail_offset;
    int copy_size, copied = size;
    char *block_data;

    if ((buf->tail == NULL) && (allocate_block(buf) != 0))
        return -1;

    while (size > 0) {
        copy_size = buf->block_size - buf->tail_offset;
        copy_size = copy_size >= size ? size : copy_size;

        block_data = (char *)(buf->tail + 1);
        memcpy(block_data + buf->tail_offset,
                src + offset,
                copy_size);

        size -= copy_size;
	offset += copy_size;
        buf->tail_offset += copy_size;
	buf->total_size += copy_size;

        if ((buf->tail_offset == buf->block_size) && (size > 0)) {
            if (allocate_block(buf) != 0) {
        		rollback_append(buf, cp_block, cp_offset);
        		copied = -1;
                break;
        	}
        }
    }

    return copied;
}

int buffer_read(struct buffer *buf, char *dest, int offset, int size)
{
    struct buffer_block *block;
    int copy_size, copied = 0;
    char *block_data;

    while (size > 0) {
        if (buf->head == NULL)
            break;

        copy_size = buf->block_size - buf->head_offset;
        copy_size = copy_size >= size ? size : copy_size;

        block_data = (char *)(buf->head + 1);
        memcpy(dest + offset,
		block_data + buf->head_offset,
                copy_size);

        size -= copy_size;
	offset += copy_size;
        copied += copy_size;
        buf->head_offset += copy_size;
	buf->total_size -= copy_size;

        if (buf->head_offset == buf->block_size) {
            block = buf->head;
            buf->head = block->next;
            buf->head_offset = 0;
            free(block);
        }
    }

    return copied;
}

void flush_buffer(struct buffer *buf)
{
    struct buffer_block *iter, *next;

	iter = buf->head;
	while (iter) {
		next = iter->next;
		free(iter);
        iter = next;
	}
}
