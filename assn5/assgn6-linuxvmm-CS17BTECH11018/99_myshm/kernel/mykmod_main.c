#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mm.h>

#include <myshm.h>

MODULE_DESCRIPTION("My kernel module - mykmod");
MODULE_AUTHOR("maruthisi.inukonda [at] gmail.com");
MODULE_LICENSE("GPL");

// Dynamically allocate major no
#define MYKMOD_DEV_MAJOR 0

static int mykmod_init_module(void);
static void mykmod_cleanup_module(void);

static int mykmod_open(struct inode *inode, struct file *filp);
static int mykmod_close(struct inode *inode, struct file *filp);
static ssize_t mykmod_read(struct file *, char *, size_t, loff_t *);
static ssize_t mykmod_write(struct file *, const char *, size_t, loff_t *);
static loff_t mykmod_lseek(struct file *, loff_t, int);
static int mykmod_mmap(struct file *filp, struct vm_area_struct *vma);

module_init(mykmod_init_module);
module_exit(mykmod_cleanup_module);

static struct file_operations mykmod_fops = {
	.owner  = THIS_MODULE,	/* owner (struct module *) */
	.open   = mykmod_open,	/* open */
	.release  = mykmod_close,     /* release */
	.read   = mykmod_read,	/* read */
	.write  = mykmod_write,	/* write */
	.llseek = mykmod_lseek, /* lseek */
	.mmap = mykmod_mmap,	/* mmap */
};

static void mykmod_vm_open(struct vm_area_struct *vma);
static void mykmod_vm_close(struct vm_area_struct *vma);
//static int mykmod_vm_fault(struct vm_fault *vmf);
static int mykmod_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf);

struct mykmod_dev_info {
	char *data;
	size_t size;
};

static const struct vm_operations_struct mykmod_vm_ops = {
	.open  = mykmod_vm_open,
	.close = mykmod_vm_close,
	.fault = mykmod_vm_fault
};

int mykmod_major;

static int mykmod_init_module(void) {

	printk("mykmod loaded\n");
	printk("mykmod initialized at=%p\n", init_module);

	if ((mykmod_major = register_chrdev(MYKMOD_DEV_MAJOR,"mykmod",&mykmod_fops))<0) {
		printk(KERN_WARNING "Failed to register character device\n");
		return 1;
	}
	else {
		printk("register character device %d\n", mykmod_major);
		printk("&fops=%p open=%p close=%p\n", &mykmod_fops, mykmod_open, mykmod_close);
	}

	return 0;
}

static void mykmod_cleanup_module(void) {

	printk("mykmod unloaded\n");
	unregister_chrdev(mykmod_major,"mykmod");

	return;
}

static int
mykmod_open(struct inode *inodep, struct file *filep)
{
	printk("mykmod_open: inodep=%p filep=%p i->private_data=%p f->private_data=%p\n",
		inodep, filep, inodep->i_private, filep->private_data);

	// TODO 1

	return 0;
}

static int
mykmod_close(struct inode *inodep, struct file *filep)
{
	printk("mykmod_close: inodep=%p filep=%p\n", inodep, filep);

	// TODO 2

	return 0;
}

static ssize_t
mykmod_read(struct file *filep, char *userbufp, size_t size, loff_t *loffp)
{
	printk("mykmod_read: filep=%p userbuf=%p size=%lu loff=%lld\n", filep, userbufp, size, *loffp);

	// TODO 3

	return 0;
}

static ssize_t
mykmod_write(struct file *filep, const char *userbufp, size_t size, loff_t *loffp)
{
	printk("mykmod_write: filep=%p userbuf=%p size=%lu loff=%lld\n", filep, userbufp, size, *loffp);

	// TODO 4

	return 0;
}

static loff_t
mykmod_lseek(struct file *filep, loff_t loff, int whence)
{
	printk("mykmod_lseek: filep=%p loff=%lld whence=%d\n", filep, loff, whence);

	// TODO 5

	return 0;
}

static int mykmod_mmap(struct file *filp, struct vm_area_struct *vma)
{
	printk("mykmod_mmap: filp=%p vma=%p\n", filp, vma);

	// TODO 6

	return 0;
}

static void
mykmod_vm_open(struct vm_area_struct *vma)
{
	printk("mykmod_vm_open: vma=%p\n", vma);

	// TODO 7
}

static void
mykmod_vm_close(struct vm_area_struct *vma)
{
	printk("mykmod_vm_close: vma=%p\n", vma);

	// TODO 8
}

static int
mykmod_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	printk("mykmod_vm_fault: vma=%p vmf=%p pgoff=%lu\n", vma, vmf, vmf->pgoff);

	// TODO 9

	return 0;
}


