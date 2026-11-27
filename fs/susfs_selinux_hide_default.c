// SPDX-License-Identifier: GPL-2.0
/*
 * Equinox FinaSU Edition: arranca con "SELinux Hide" activo y oculta a las apps
 * los tipos de ADB root de la ROM.
 *
 * 1) SELinux Hide: FinaSU solo lo enciende si hay una configuracion guardada con
 *    ksud. Sin ella, el kernel descarta la copia de la politica SELinux original al
 *    terminar el arranque y despues ya no puede encenderlo ("no backup sepolicy
 *    available"), asi que una app puede comprobar que existen los contextos ksu.
 *    Llamamos a la API de funciones de FinaSU (ksu_set_feature) poco despues del
 *    arranque; un selinux_hide=0 guardado por el usuario se respeta (ksud lo apaga).
 *
 * 2) adbroot: la ROM (MistOS) define los tipos "adbroot"/"adbroot_exec". Una app
 *    puede confirmar que existen escribiendo su contexto en selinuxfs, y detectores
 *    como Duck Detector lo marcan como "privilegios de ADB root". Los registramos en
 *    la lista de tipos ocultos de SUSFS (igual que se hace con ksu), de modo que las
 *    consultas de una app ya no los confirmen. No cambia la politica real: el ADB
 *    root de la ROM sigue funcionando.
 *
 * Nada de esto toca el codigo oficial de FinaSU.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include <linux/types.h>

#define SUSFS_FEATURE_SELINUX_HIDE	4	/* KSU_FEATURE_SELINUX_HIDE, KernelSU/uapi/feature.h */
#define SUSFS_HIDE_TRIES		30	/* 1 intento por segundo */

extern int ksu_set_feature(u32 feature_id, u64 value);
extern int susfs_register_existing_sus_type(const char *type_name);

/* Tipos de la ROM que delatan "ADB root"; se ocultan a las apps si existen. */
static const char * const susfs_rom_sus_types[] = {
	"adbroot",
	"adbroot_exec",
};

static void susfs_selinux_hide_default_fn(struct work_struct *work);
static DECLARE_DELAYED_WORK(susfs_selinux_hide_default_work, susfs_selinux_hide_default_fn);
static int susfs_hide_tries;
static bool susfs_hide_done;
static bool susfs_rom_types_done;

static bool susfs_register_rom_types(void)
{
	bool all = true;
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(susfs_rom_sus_types); i++) {
		ret = susfs_register_existing_sus_type(susfs_rom_sus_types[i]);
		if (ret == -ENOENT) {
			/* tipo ausente en esta ROM: nada que ocultar, no es error */
			pr_info("susfs: rom type '%s' not in policy, skip\n",
				susfs_rom_sus_types[i]);
		} else if (ret) {
			all = false; /* reintentar luego */
		} else {
			pr_info("susfs: rom type '%s' hidden from apps\n",
				susfs_rom_sus_types[i]);
		}
	}
	return all;
}

static void susfs_selinux_hide_default_fn(struct work_struct *work)
{
	if (!susfs_hide_done) {
		int ret = ksu_set_feature(SUSFS_FEATURE_SELINUX_HIDE, 1);

		if (!ret) {
			susfs_hide_done = true;
			pr_info("susfs: selinux_hide enabled by default\n");
		} else if (ret != -EAGAIN) {
			/* el usuario o un modulo lo gestionan; no insistir en el hide */
			susfs_hide_done = true;
			pr_info("susfs: selinux_hide default not applied (%d)\n", ret);
		}
	}

	if (!susfs_rom_types_done)
		susfs_rom_types_done = susfs_register_rom_types();

	if ((susfs_hide_done && susfs_rom_types_done) ||
	    ++susfs_hide_tries >= SUSFS_HIDE_TRIES)
		return;

	schedule_delayed_work(&susfs_selinux_hide_default_work, HZ);
}

static int __init susfs_selinux_hide_default_init(void)
{
	schedule_delayed_work(&susfs_selinux_hide_default_work, HZ);
	return 0;
}
late_initcall(susfs_selinux_hide_default_init);
