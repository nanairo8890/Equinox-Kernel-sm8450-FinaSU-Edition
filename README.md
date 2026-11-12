<div align="center">

<img src="finasu-edition-banner.webp" alt="Equinox FinaSU Edition by Nanairo8890" width="100%">

# Equinox FinaSU Edition by Nanairo8890

**POCO F5 Pro / Redmi K60** · `mondrian` · Snapdragon 8+ Gen 1 · **MistOS 5.1 Beta**

**Developer:** [LF52406](https://github.com/LF52406) · **[Download builds](../../releases)**

</div>

---

> [!NOTE]
> **This repository is the FinaSU edition of Equinox**, made by **Nanairo** with
> LF52406's permission. It is Equinox 5.10.269 as released, with these changes:
>
> - **[FinaSU](https://github.com/LyraVoid/FinaSU)** by LyraVoid (a fork of [KernelSU](https://github.com/tiann/KernelSU)) instead of KernelSU-Next, as the `KernelSU/` submodule at `14a71d95` (`v1.6.1`, version 33198), built into the kernel with FinaSU's default Tracepoint hook backend. Use the **official FinaSU manager** (`FinaSU_v1.6.1_33198-release.apk`, package `me.yuki.fina`): the kernel trusts that manager's signing certificate in FinaSU's release trust slot, and other KernelSU-family managers are recognized but never granted. FinaSU uses UAPI 6.
> - **SUSFS v2.4.0** from [simonpunk's](https://gitlab.com/simonpunk/susfs4ksu) `gki-android12-5.10` branch, through FinaSU's SUSFS support. FinaSU v1.6.0 and later register the SELinux types and rules KernelSU injects as SUSFS hidden entries themselves, so SELinux queries from apps do not see them.
> - **BBR** as the default TCP congestion control.
> - **[NoMount](https://github.com/maxsteeel/nomount) v2.2.0** by maxsteeel built in (`fs/nomount`), which includes deref3046's fix. FinaSU detects the built-in driver and uses it as the **NoMount** mount backend, so **no metamodule is needed** (choose NoMount in the manager; remove the NoMount metamodule if installed). Processes that FinaSU/SUSFS umount modules for see the stock filesystem, and NoMount is hidden from `/sys/module` and `/proc/kallsyms`.
> - **Extra `/proc/kallsyms` hiding** (r2, extended in 1.8): FinaSU exports many driver symbols whose names do not start with the usual KernelSU/SUSFS prefixes, so SUSFS's filter left them visible. The filter now also hides those, each one checked against a clean Equinox build; the prefixes added in 1.8 were also checked against the vendor modules of MistOS 20261025 and Infinity-X 4.1, so only FinaSU's own symbols are hidden.
>
> Everything else, DroidSpaces included, is LF52406's work, described below. The official Equinox
> builds are at [LF52406/Equinox-Kernel-sm8450](https://github.com/LF52406/Equinox-Kernel-sm8450).
> Report problems with this edition to Nanairo8890, not to LF52406 or LyraVoid.
>
> Build: `CLANG_DIR=/path/to/clang/bin scripts/equinox/build-finasu.sh`, then `scripts/equinox/package-finasu.sh` · Changes: [release notes](scripts/equinox/release-notes/5.10.269-finasu-2.1.md) · Changelog: [English](CHANGELOG.en.txt) · [Español](CHANGELOG.es.txt) · [Português (Brasil)](CHANGELOG.pt-BR.txt) · [Русский](CHANGELOG.ru.txt)

---

## Current Build

| Component | Current |
|:--|:--|
| **Version** | `2.1` |
| **Linux** | `5.10.269-Nanairo-FinaSU` |
| **Compatibility** | MistOS and the AOSP ROMs listed in [Other AOSP ROMs](#other-aosp-roms); tested on MistOS 5.1 Beta 20261025 (`fastboot boot`) |
| **FinaSU** | `v1.6.1` (33198), built in · Tracepoint hooks |
| **SUSFS** | `2.4.0` |
| **NoMount** | `v2.2.0` built in |
| **DroidSpaces** | Supported |
| **TCP** | BBR default · CUBIC available |
| **Toolchain** | AOSP Clang `22.0.0` (r584948) |
| **Build** | ThinLTO · Clang CFI · MODVERSIONS |
| **KMI** | Verified against clean Equinox 5.10.269 |

---

## Features

| Group | Included |
|:--|:--|
| **Root & hiding** | FinaSU integration · SUSFS · extra `/proc/kallsyms` hiding |
| **Containers** | DroidSpaces · PID namespaces · IPC / SYSVIPC |
| **Networking** | TCP BBR · runtime congestion-control switching |
| **Device** | mondrian production config · Goodix touch stability fixes |
| **Packaging** | Verified AnyKernel3 release package · vendor module check before flashing |

---

## Latest Changes

### 2.1

- FinaSU updated from `v1.5.2` to `v1.6.1` (33198); use the FinaSU `v1.6.1` manager
- SELinux hiding is now done by FinaSU itself: every type and allow rule KernelSU injects is registered as a SUSFS hidden entry, so the patch that 2.0.1 applied at build time is gone and the submodule is built as released
- FinaSU now dispatches the SUSFS priority UID filter, so BRENE should see all 10 SUSFS functions
- Same KMI as 2.0 and 2.0.1, same NoMount v2.2.0, same installer and same ROM compatibility

### 2.0.1

- Hides KernelSU's own SELinux types (`ksu`, `ksu_file`) from apps through SUSFS, as a patch applied over the KernelSU submodule at build time; same KMI as 2.0

### 2.0

- FinaSU updated from `v1.5.0` to `v1.5.1` (33177); use the FinaSU `v1.5.1` manager
- Kernel side of FinaSU `v1.5.1` that applies here: SELinux hiding parses the context a process writes once, against the live policy, and handles blank and newline-only contexts correctly
- Everything else is the same as 1.9: Tracepoint hooks, SUSFS `eb95da0` (v2.3.0, v2.4.0 is not supported by FinaSU yet), NoMount v2.2.0, same installer and same ROM compatibility

### 1.9

- FinaSU updated from `v1.4.4` to `v1.5.0` (33174); use the FinaSU `v1.5.0` manager
- Kernel side of FinaSU `v1.5.0`: two SELinux fixes (policy generation tracking when the policy is updated at run time, and marking the original SELinux types for SELinux hiding)
- Everything else is the same as 1.8: Tracepoint hooks, SUSFS `eb95da0` (v2.3.0, v2.4.0 is not supported by FinaSU yet), NoMount v2.2.0, same installer and same ROM compatibility

### 1.8

- FinaSU updated from `v1.2.1` to `v1.4.4` (33167); use the FinaSU `v1.4.4` manager. With FinaSU's 1.4 userspace, the r2 kernel could not list the kernel umount paths or report the hook type; 1.8 can
- Hooks: FinaSU's default Tracepoint backend, as before (the new SUSFS Inline backend is not enabled)
- SUSFS stays at `eb95da0` (v2.3.0): newer SUSFS commits change SELinux hiding in a way FinaSU does not support yet
- `/proc/kallsyms` filter extended to the symbols FinaSU added since v1.2.1 (the vivo module filter and `get_try_umount`) and to older FinaSU symbols that r2 still showed
- Same installer as 1.5, with the vendor module check; 1.5 was not published as a separate release
- Exports the same symbol CRCs as r2 and 1.5, so the ROM compatibility below still holds; tested on the phone with MistOS 5.1 Beta 20261025 (`fastboot boot`, not flashed), where the installer's `kmicheck` was also run

### 1.5

- Same kernel as r2 (`5.10.269-Nanairo-FinaSU`, identical `Image`); this version changes the installer
- The installer checks the ROM's `vendor_boot` and `vendor_dlkm` modules against the kernel before flashing and stops without writing anything if they would not load
- `checkonly` package: tells whether a ROM is compatible and flashes nothing
- Checked compatible: MistOS, the latest builds from Keosh (crDroid, Evolution X, Infinity-X, Lunaris, LineageOS, Axion), Infinity-X 4.1 by LF5 and crDroid 12.12 by Taha Keles; not compatible: AOSPA and PenguinOS

### 5.10.269 r2

- FinaSU `v1.2.1` (33074) built in, with the official FinaSU manager trusted in the release signature slot
- SUSFS updated to `eb95da0` (v2.3.0), which fixes a use-after-free in the priority-uid filter
- NoMount `v2.2.0` by maxsteeel built in; used as FinaSU's built-in NoMount backend, no metamodule needed
- Extra `/proc/kallsyms` hiding for FinaSU's own driver symbols, validated against clean Equinox
- Kernel named `5.10.269-Nanairo-FinaSU`
- Vendor module CRCs checked against MistOS 5.1 Beta 20261025, 20261024, 20261023 and 20261011, and MistOS 5.0 Alpha 20260914
- KMI verified against clean Equinox 5.10.269

[View full release notes](../../releases)

---

## Downloads & Flashing

Builds are published through **GitHub Releases**, with the SHA256 of each file on the release page.

**Current package:** `Equinox-5.10.269-FinaSU-2.0-mondrian.zip`

Download the release asset and flash the AnyKernel3 ZIP using a compatible recovery or kernel flasher. The ZIP only replaces the kernel `Image`, after checking that the ROM's vendor modules accept it; if you update the ROM, flash it again afterwards. Install the FinaSU manager from the release to manage root, modules and SUSFS.

Not sure whether your ROM is compatible? Flash `Equinox-5.10.269-FinaSU-2.0-mondrian-checkonly.zip` first: it runs the same check and flashes nothing.

> GitHub-generated `Source code` archives are not flashable kernel packages.

---

## Other AOSP ROMs

The ZIP keeps the ROM's vendor modules (`vendor_boot` and `vendor_dlkm`), and those only load on a kernel with the same symbol CRCs (KMI) as the kernel they were built with. A single `Image` therefore works on the ROMs whose kernel comes from the same source as Equinox, not on every ROM:

| ROM | ROM kernel | Status |
|:--|:--|:--|
| MistOS 5.1 Beta (20261011 to 20261025) | 5.10.269 | Tested on 20261025 (`fastboot boot`); the CRCs of all 337 `vendor_boot` and 390 `vendor_dlkm` modules match |
| MistOS 5.0 Alpha 20260914 | `5.10.256-gki-gb48a614d5811` | Compatible: the CRCs of all 337 `vendor_boot` and 390 `vendor_dlkm` modules match |
| Latest builds from Keosh: crDroid 12.12 (20260928), Evolution X 11.11, Infinity-X 3.12 (VANILLA, GAPPS core and extra, 29.09.2026), Lunaris 3.12 (VANILLA and GMS), LineageOS 23.2 (20260929), Axion 2.8 (VANILLA and GMS) | `5.10.269-gki-g1598dc02ea84` | Compatible: the CRCs of all 337 `vendor_boot` and 390 `vendor_dlkm` modules match (checked with r2 and with 2.0) |
| Infinity-X 4.1 by LF5 (17.10.2026) | `5.10.269-gki-g283772ceb844` | Compatible: the CRCs of all 337 `vendor_boot` and 390 `vendor_dlkm` modules match (checked with r2 and with 2.0) |
| crDroid 12.12 by Taha Keles (20260930) | `5.10.260-Weil+` | Compatible: all 725 non-blocklisted modules match the r2 kernel (`kmicheck`), which exports the same CRCs as 2.0 |
| AOSPA calcite (a17-gms 20261012, a17-microg 20261013), PenguinOS celerity (20261011) | `5.10.269-beryl-7156c6f9fb`, their own | **Not compatible**: 376 + 279 `vendor_boot` / `vendor_dlkm` modules, almost all with other CRCs |
| LineageOS `lineage-24.0` (Android 17) and `lineage-23.2` (Android 16 QPR2), and ROMs built on them | 5.10.269 | Expected to work: same tree as Equinox |
| LineageOS `lineage-23.0` / `lineage-23.1` (Android 16) | 5.10.247 | Not verified |
| LineageOS `lineage-22.2` (Android 15) | 5.10.245 | Not verified |
| HyperOS / MIUI | | Not supported |

Packages made with `scripts/equinox/package-finasu.sh` check this before flashing: `kmicheck` reads the ROM's modules from `vendor_boot` and `vendor_dlkm` and compares them with the kernel's own symbol table (`kmi.symvers`). If any module would be rejected (different CRC, a symbol this kernel does not export, a duplicate export or a different vermagic), the installer stops before writing anything and lists the modules. A ZIP with `nocheck` in its file name skips the check; one with `checkonly` runs the check and flashes nothing, to find out whether a ROM is compatible. Released packages from 1.8 on include this check (1.5 was not released separately); r2 does not.

---

## Reporting Issues

When reporting a problem to Nanairo8890, include the kernel version, a short reproduction description and relevant logs such as `dmesg`, `logcat`, `pstore / ramoops`, or watchdog / panic logs when available.

---

## Support Development

If you find **Equinox Kernel** useful and would like to support its continued development, donations are always appreciated but entirely optional.

<div align="center">

[![PayPal](https://img.shields.io/badge/PayPal-Support-003087?style=for-the-badge&logo=paypal&logoColor=white)](https://www.paypal.me/LF52406)
[![Patreon](https://img.shields.io/badge/Patreon-Support-FF424D?style=for-the-badge&logo=patreon&logoColor=white)](https://patreon.com/InfernalGT)
[![Boosty](https://img.shields.io/badge/Boosty-Support-F15F2C?style=for-the-badge&logo=boosty&logoColor=white)](https://boosty.to/infernalgt/donate)

</div>

---

## Support Nanairo

If this FinaSU edition is useful to you and you would like to support Nanairo8890's work on it (the FinaSU, SUSFS and NoMount integration, and the vendor module check), donations are welcome but entirely optional.

<div align="center">

[![Ko-fi](https://img.shields.io/badge/Ko--fi-Support-FF5E5B?style=for-the-badge&logo=kofi&logoColor=white)](https://ko-fi.com/nanairo8890)

</div>

---

## Credits & Upstreams

- [**LF52406**](https://github.com/LF52406) — Equinox Kernel, the base of this edition
- [**LineageOS/android_kernel_xiaomi_sm8450**](https://github.com/LineageOS/android_kernel_xiaomi_sm8450) — primary kernel base used by Equinox
- [**LineageOS/android_kernel_qcom_sm8450**](https://github.com/LineageOS/android_kernel_qcom_sm8450) — Qualcomm common-kernel upstream used by Equinox
- [**Android Common Kernel**](https://android.googlesource.com/kernel/common) / [**Linux Kernel**](https://www.kernel.org/) — Android kernel infrastructure and upstream Linux
- [**FinaSU**](https://github.com/LyraVoid/FinaSU) by **LyraVoid**, based on [**KernelSU**](https://github.com/tiann/KernelSU) by **tiann** — kernel root implementation of this edition
- [**SUSFS**](https://gitlab.com/simonpunk/susfs4ksu) by **simonpunk** — SUSFS integration for Linux 5.10
- [**DroidSpaces**](https://github.com/ravindu644/Droidspaces-OSS) by **ravindu644** — container runtime and kernel requirements
- [**NoMount**](https://github.com/maxsteeel/nomount) by **maxsteeel**, with a fix by **deref3046** — VFS path redirection, built in as FinaSU's NoMount backend
- [**AnyKernel3**](https://github.com/osm0sis/AnyKernel3) by **osm0sis** — flashable kernel packaging
- [**Nanairo8890**](https://github.com/nanairo8890) — FinaSU, SUSFS, BBR and NoMount integration of this edition

Thanks to Qualcomm, LineageOS contributors, Android Common Kernel contributors, and the broader Linux kernel community for the upstream kernel, platform, and driver work that Equinox builds upon.
