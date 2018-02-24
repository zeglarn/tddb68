#include "userprog/syscall.h"
#include <stdio.h>
#include <console.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"

static void syscall_handler (struct intr_frame *);
bool create (void *esp);
int open(void *esp);
void close(void *esp);
int read(void *esp);
int write(void *esp);




void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  // Get syscall nr.
  uint32_t syscall_no = *((uint32_t *) f->esp);

  switch (syscall_no) {
    case SYS_HALT:
    power_off();
    break;

    case SYS_CREATE:
    f->eax = create(f->esp);
    break;

    case SYS_OPEN:
    f->eax = open(f->esp);
    break;

    case SYS_CLOSE:
    close(f->esp);
    break;

    case SYS_READ:
    f->eax = read(f->esp);
    break;

    case SYS_WRITE:
    f->eax = write(f->esp);
    break;

    case SYS_EXIT:
    thread_exit();
    break;

    case SYS_EXEC:
    f->eax = exec(f->esp);
    break;
  }
  //printf ("system call!\n");
  //thread_exit ();
}

bool create(void *esp) {
  const char *file = *((const char **) (esp + 4));
  uint32_t initial_size = *((uint32_t *) (esp + 8));
  return filesys_create(file, initial_size);
}

int open(void *esp) {
  const char *file = *((const char **) (esp + 4));
  struct thread *thread = thread_current();

  int i = 0;
  for ( i ; i < FDSIZE; i = i + 1) {
    if (thread->fds[i] == NULL) {
      break;
    }
  }

  struct file *f = filesys_open(file);
  if (f) {
    thread->fds[i] = f;
    return i + 2;
  }
  return -1;
}

void close(void *esp) {
  uint32_t fd = *((uint32_t *) (esp + 4));
  struct thread *thread = thread_current();
  file_close(thread->fds[fd - 2]);
  thread->fds [fd - 2] = NULL;
}


int read(void *esp) {
  uint32_t fd = *((uint32_t *) (esp + 4));
  void *buffer = *((void **) (esp + 8));
  uint32_t size = *((uint32_t *) (esp + 12));


  if (fd == 0) {
    int i = 0;
    for ( i; i < size; i = i + 1) {
      uint8_t key = input_getc();
      buffer = key;
      buffer = buffer + sizeof(uint8_t);
      printf("%c", key);
    }
    printf("\n");
    return i;
  }

  if (fd > FDSIZE || fd < 2)
  return -1;

  struct thread *thread = thread_current();

  if (!thread->fds[fd - 2]) {
    return -1;
  }

  return file_read(thread->fds[fd - 2], buffer, size);
}

int write(void *esp) {
  uint32_t fd = *((uint32_t *) (esp + 4));
  const void *buffer = *((void **) (esp + 8));
  uint32_t size = *((uint32_t *) (esp + 12));

  struct thread *thread = thread_current();

  if (fd < 1 || fd > FDSIZE)
    return -1;
  if (fd == 1) {
    uint32_t offset = 50;
    const char *ch = (const char *) buffer;
    while(size > offset) {
      putbuf(ch, offset);
      size = size - offset;
      ch = ch + offset;
    }
    putbuf(ch, size);
    return *((uint32_t *) (esp + 12));
  }

  if (!thread->fds[fd - 2])
    return -1;

  return file_write( thread->fds[fd - 2], buffer, size);
}

pid_t exec (const char *cmd_line)  {
  
}

void exit(int status) {
  thread_current()->ctxt->exit_status = status;
  thread_exit();
}
