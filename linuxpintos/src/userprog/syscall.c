#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"

static void syscall_handler (struct intr_frame *);
bool create (void *esp);


void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  // Get syscall nr.
  uint32_t syscall_no = (uint32_t) f->esp;

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

    case
  }
  //printf ("system call!\n");
  //thread_exit ();
}

bool create(void *esp) {
  const char *file = (const char *) (esp + 4);
  uint32_t initial_size = *((uint32_t *) (esp + 8));
  return filesys_create(file, initial_size);
}

int open(void *esp) {
  const char *file = (const char *) (esp + 4);
  struct file *f = NULL;
  struct thread *thread = thread_current();

  int i = 0;
  for ( i ; i < FDSIZE; i = i +1) {
    if (thread->fds[i] == NULL) {
      struct file *f = filesys_open(file);
      if (f) {
        thread->fds[i] = f;
        return i + 2;
      } else {
        return -1;
      }
    }
  }
  return -1;
}

void close(void *esp) {
  uint32_t fd = *((uint32_t *) (esp + 4));
  struct thread *thread = thread_current();
  file_close(thread->fds[fd - 2]);
  thread->fds [fd - 2] = NULL;
}


//TODO När ska vi returnera -1?
int read(void *esp) {
  uint32_t fd = *((uint32_t *) (esp + 4));
  if (fd == 0) {
    return input_getc();
  }

  void *buffer = ((void *) (esp + 8));
  uint32_t size = *((uint32_t *) (esp + 12));
  struct thread *thread = thread_current();
  return file_read(thread->fds[fd -2], buffer, size);
}
