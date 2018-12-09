/*
 * Copyright 2009 VIA Technologies, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include <sys/types.h>
#include <sys/io.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct bit_desc {
	uint8_t mask;
	char *name;
};

struct io_index {
	char *name;
	struct bit_desc *bit_desc;
};

struct io_reg {
	uint16_t	io_port_addr;	/* port for address */
	uint16_t	io_port_data;	/* port for address */
	char *		name;
	struct io_index	index[0xff];
};

struct io_reg attr_regs = {
	.io_port_addr = 0x3c0,
	.io_port_data = 0x3c1,
	.name = "Attribute Controller",
	.index = {
		[0x00] = { "Palette 0", },
		[0x01] = { "Palette 1", },
		[0x02] = { "Palette 2", },
		[0x03] = { "Palette 3", },
		[0x04] = { "Palette 4", },
		[0x05] = { "Palette 5", },
		[0x06] = { "Palette 6", },
		[0x07] = { "Palette 7", },
		[0x08] = { "Palette 8", },
		[0x09] = { "Palette 9", },
		[0x0a] = { "Palette a", },
		[0x0b] = { "Palette b", },
		[0x0c] = { "Palette c", },
		[0x0d] = { "Palette d", },
		[0x0e] = { "Palette e", },
		[0x0f] = { "Palette f", },
		[0x10] = { "Mode Control", },
		[0x11] = { "Overscan Color", },
		[0x12] = { "Color Plane Enable", },
		[0x13] = { "Horizontal Pixel Panning", },
		[0x14] = { "Color Select", },
	},
};

struct io_reg graphic_regs = {
	.io_port_addr = 0x3ce,
	.io_port_data = 0x3cf,
	.name = "Graphic Controller",
	.index = {
		[0x00] = { "Set / Reset", },
		[0x01] = { "Enable Set / Reset", },
		[0x02] = { "Color Compare", },
		[0x03] = { "Data Rotate", },
		[0x04] = { "Read Map Select", },
		[0x05] = { "Mode", },
		[0x06] = { "Miscellaneous", },
		[0x07] = { "Color Don't Care", },
		[0x08] = { "Bit Mask", },
		/* Extended */
		[0x20] = { "Offset Register Control", },
		[0x21] = { "Offset Register A", },
		[0x22] = { "Offset Register B", },
	},
};


static struct bit_desc crtc_32_desc[] = {
	{ 0x01, "Real-Time Flipping", },
	{ 0x02, "Digital Video Port (DVP) Grammar Correction", },
	{ 0x04, "Display End Blanking Enable", },
	{ 0x08, "CRT SYNC Driving Selection (0: Low, 1: High)", },
	{ 0xE0, "HSYNC Delay Number by VCLK", },
	{ 0 },
};

struct io_reg crtc_regs = {
	.io_port_addr = 0x3d4,
	.io_port_data = 0x3d5,
	.name = "CRT controller",
	.index = {
		/* CRT Controller registers */
		[0x00] = { "Horizontal Total", },
		[0x01] = { "Horizontal Display End", },
		[0x02] = { "Start Horizontal Blank", },
		[0x03] = { "End Horizontal Blank", },
		[0x04] = { "Start Horizontal Retrace", },
		[0x05] = { "End Horizontal Retrace", },
		[0x06] = { "Vertical Total", },
		[0x07] = { "Overflow", },
		[0x08] = { "Preset Row Scan", },
		[0x09] = { "Max Scan Line", },
		[0x0a] = { "Cursor Start", },
		[0x0b] = { "Cursor End", },
		[0x0c] = { "Start Address High", },
		[0x0d] = { "Start Address Low", },
		[0x0e] = { "Cursor Location High", },
		[0x0f] = { "Cursor Location Low", },
		[0x10] = { "Vertical Retrace Start", },
		[0x11] = { "Vertical Retrace End", },
		[0x12] = { "Vertical Display End", },
		[0x13] = { "Offset", },
		[0x14] = { "Underline Location", },
		[0x15] = { "Start Vertical Blank", },
		[0x16] = { "End Vertical Blank", },
		[0x17] = { "CRTC Mode Control", },
		[0x18] = { "Line Compare", },
		/* CRT Controller Extended Register */
		[0x30] = { "Display Fetch Blocking Control", },
		[0x31] = { "Half Line Position", },
		[0x32] = { "Mode Control", crtc_32_desc, },
		[0x33] = { "Hsync Adjuster", },
		[0x34] = { "Starting Address Overflow, Bits [23:16]", },
		[0x35] = { "Extended Overflow", },
		[0x36] = { "Power Management Control 3", },
		[0x37] = { "DAC Control", },
		[0x38] = { "Signature Data B0", },
		[0x39] = { "Signature Data B1", },
		[0x3a] = { "Signature Data B2", },
		[0x3b] = { "Scratch Pad 2", },
		[0x3c] = { "Scratch Pad 3", },
		[0x3d] = { "Scratch Pad 4", },
		[0x3e] = { "Scratch Pad 5", },
		[0x3f] = { "Scratch Pad 6", },
		[0x40] = { "Test Mode Control 0", },
		[0x43] = { "IGA1 Display Control", },
		[0x45] = { "Power Now Indicator Control 3", },
		[0x46] = { "Test Mode Control 1", },
		[0x47] = { "Test Mode Control 2", },
		[0x48] = { "Starting Address Overflow", },
		/* Sequencer Extended Registers */
		[0x50] = { "Second CRTC Horizontal Total Period", },
		[0x51] = { "Second CRTC Horizontal Active Data Period", },
		[0x52] = { "Second CRTC Horizontal Blanking Start", },
		[0x53] = { "Second CRTC Horizontal Blanking End", },
		[0x54] = { "Second CRTC Horizontal Blanking Overflow", },
		[0x55] = { "Second CRTC Horizontal Period Overflow", },
		[0x56] = { "Second CRTC Horizontal Retrace Start", },
		[0x57] = { "Second CRTC Horizontal Retrace End", },
		[0x58] = { "Second CRTC Vertical Total Period", },
		[0x59] = { "Second CRTC Vertical Active Data Period", },
		[0x5a] = { "Second CRTC Vertical Blanking Start", },
		[0x5b] = { "Second CRTC Vertical Blanking End", },
		[0x5c] = { "Second CRTC Vertical Blanking Overflow", },
		[0x5d] = { "Second CRTC Vertical Period Overflow", },
		[0x5e] = { "Second CRTC Vertical Retrace Start", },
		[0x5f] = { "Second CRTC Vertical Retrace End", },
		[0x60] = { "Second CRTC Vertical Status 1", },
		[0x61] = { "Second CRTC Vertical Status 2", },
		[0x62] = { "Second Display Starting Address Low", },
		[0x63] = { "Second Display Starting Address Middle", },
		[0x64] = { "Second Display Starting Address High", },
		[0x65] = { "Second Display Horizontal Quadword Count", },
		[0x66] = { "Second Display Horizontal Offset", },
		[0x67] = { "Second Display Color Depth and Horizontal Overflow", },
		[0x68] = { "Second Display Queue Depth and Read Threshold", },
		[0x69] = { "Second Display Interrupt Enable and Status", },
		[0x6a] = { "Second Display Channel and LCD Enable", },
		[0x6b] = { "Channel 1 and 2 Clock Mode Selection", },
		[0x6c] = { "TV Clock Control", },
		[0x6d] = { "Horizontal Total Shadow", },
		[0x6e] = { "End Horizontal Blanking Shadow", },
		[0x6f] = { "Vertical Total Shadow", },
		[0x70] = { "Vertical Display Enable End Shadow", },
		[0x71] = { "Vertical Display Overflow Shadow", },
		[0x72] = { "Start Vertical Blank Shadow", },
		[0x73] = { "End Vertical Blank Shadow", },
		[0x74] = { "Vertical Blank Overflow Shadow", },
		[0x75] = { "Vertical Retrace Start Shadow", },
		[0x76] = { "Vertical Retrace End Shadow", },
		[0x77] = { "LCD Horizontal Scaling Factor", },
		[0x78] = { "LCD Vertical Scaling Factor", },
		[0x79] = { "LCD Scaling Control", },
		[0x7a] = { "LCD Scaling Parameter 1", },
		[0x7b] = { "LCD Scaling Parameter 2", },
		[0x7c] = { "LCD Scaling Parameter 3", },
		[0x7d] = { "LCD Scaling Parameter 4", },
		[0x7e] = { "LCD Scaling Parameter 5", },
		[0x7f] = { "LCD Scaling Parameter 6", },
		[0x80] = { "LCD Scaling Parameter 7", },
		[0x81] = { "LCD Scaling Parameter 8", },
		[0x82] = { "LCD Scaling Parameter 9", },
		[0x83] = { "LCD Scaling Parameter 10", },
		[0x84] = { "LCD Scaling Parameter 11", },
		[0x85] = { "LCD Scaling Parameter 12", },
		[0x86] = { "LCD Scaling Parameter 13", },
		[0x87] = { "LCD Scaling Parameter 14", },
		[0x88] = { "LCD Panel Type", },
		[0x8a] = { "LCD Timing Control 1", },
		[0x8b] = { "LCD Power Sequence Control 0", },
		[0x8c] = { "LCD Power Sequence Control 1", },
		[0x8d] = { "LCD Power Sequence Control 2", },
		[0x8e] = { "LCD Power Sequence Control 3", },
		[0x8f] = { "LCD Power Sequence Control 4", },
		[0x90] = { "LCD Power Sequence Control 5", },
		[0x91] = { "Software Control Power Sequence", },
		[0x92] = { "Read Threshold 2", },
		[0x94] = { "Expire Number and Display Queue Extend", },
		[0x95] = { "Extend Threshold Bit", },
		[0x97] = { "LVDS Channel 2 Function Select 0", },
		[0x98] = { "LVDS Channel 2 Function Select 1", },
		[0x99] = { "LVDS Channel 1 Function Select 0", },
		[0x9a] = { "LVDS Channel 1 Function Select 1", },
		[0x9b] = { "Digital Video Port 1 Function Select 0", },
		[0x9c] = { "Digital Video Port 1 Function Select 1", },
		[0x9d] = { "Power Now Control 2", },
		[0x9e] = { "Power Now Control 3", },
		[0x9f] = { "Power Now Control 4", },
		[0xa0] = { "Horizontal Scaling Initial Value", },
		[0xa1] = { "Vertical Scaling Initial Value", },
		[0xa2] = { "Horizontal and Vertical Scaling Enable", },
		[0xa3] = { "Second Display Starting Address Extended", },
		[0xa5] = { "Second LCD Vertical Scaling Factor", },
		[0xa6] = { "Second LCD Vertical Scaling Factor", },
		[0xa7] = { "Expected IGA1 Vertical Display End", },
		[0xa8] = { "Expected IGA1 Vertical Display End", },
		[0xa9] = { "Hardware Gamma Control", },
		[0xaa] = { "FIFO Depth + Threshold Overflow", },
		[0xab] = { "IGA2 Interlace Half Line", },
		[0xac] = { "IGA2 Interlace Half Line", },
		[0xaf] = { "P-Arbiter Write Expired Number", },
		[0xb0] = { "IGA2 Pack Circuit Request Threshold", },
		[0xb1] = { "IGA2 Pack Circuit Request High Threshold", },
		[0xb2] = { "IGA2 Pack Circuit Request Expire Threshold", },
		[0xb3] = { "IGA2 Pack Circuit Control", },
		[0xb4] = { "IGA2 Pack Circuit Target Base Address 0", },
		[0xb5] = { "IGA2 Pack Circuit Target Base Address 0", },
		[0xb6] = { "IGA2 Pack Circuit Target Base Address 0", },
		[0xb7] = { "IGA2 Pack Circuit Target Base Address 0", },
		[0xb8] = { "IGA2 Pack Circuit Target Line Pitch", },
		[0xb9] = { "IGA2 Pack Circuit Target Line Pitch", },
		[0xba] = { "V Counter Set Pointer", },
		[0xbb] = { "V Counter Set Pointer", },
		[0xbc] = { "V Counter Reset Value", },
		[0xbd] = { "V Counter Reset Value", },
		[0xbe] = { "Frame Buffer Limit Value", },
		[0xbf] = { "Frame Buffer Limit Value", },
		[0xc0] = { "Expected IGA1 Vertical Display End 1", },
		[0xc1] = { "Expected IGA1 Vertical Display End 1", },
		[0xc2] = { "Third LCD Vertical Scaling Factor", },
		[0xc3] = { "Third LCD Vertical Scaling Factor", },
		[0xc4] = { "Expected IGA1 Vertical Display End 2", },
		[0xc5] = { "Expected IGA1 Vertical Display End 2", },
		[0xc7] = { "Fourth LCD Vertical Scaling Factor", },
		[0xc8] = { "IGA2 Pack Circuit Target Base Address 1", },
		[0xc9] = { "IGA2 Pack Circuit Target Base Address 1", },
		[0xca] = { "IGA2 Pack Circuit Target Base Address 1", },
		[0xcb] = { "IGA2 Pack Circuit Target Base Address 1", },
		[0xd0] = { "LVDS PLL1 Control", },
		[0xd1] = { "LVDS PLL2 Control", },
		[0xd2] = { "LVDS Control", },
		[0xd3] = { "LVDS Second Power Sequence Control 0", },
		[0xd4] = { "LVDS Second Power Sequence Control 1", },
		[0xd5] = { "LVDS Texting Mode Control", },
		[0xd6] = { "DCVI Control Register 0", },
		[0xd7] = { "DCVI Control Register 1", },
		[0xd9] = { "Scaling Down Source Data Offset Control", },
		[0xda] = { "Scaling Down Source Data Offset Control", },
		[0xdb] = { "Scaling Down Source Data Offset Control", },
		[0xdc] = { "Scaling Down Vertical Scale Control", },
		[0xdd] = { "Scaling Down Vertical Scale Control", },
		[0xde] = { "Scaling Down Vertical Scale Control", },
		[0xdf] = { "Scaling Down Vertical Scale Control", },
		[0xe0] = { "Scaling Down Destination FB Starting Addr 0", },
		[0xe1] = { "Scaling Down Destination FB Starting Addr 0", },
		[0xe2] = { "Scaling Down Destination FB Starting Addr 0", },
		[0xe3] = { "Scaling Down Destination FB Starting Addr 0", },
		[0xe4] = { "Scaling Down SW Source FB Stride", },
		[0xe5] = { "Scaling Down Destination FB Starting Addr 1", },
		[0xe6] = { "Scaling Down Destination FB Starting Addr 1", },
		[0xe7] = { "Scaling Down Destination FB Starting Addr 1", },
		[0xe8] = { "Scaling Down Destination FB Starting Addr 1", },
		[0xe9] = { "Scaling Down Destination FB Starting Addr 2", },
		[0xea] = { "Scaling Down Destination FB Starting Addr 2", },
		[0xeb] = { "Scaling Down Destination FB Starting Addr 2", },
		[0xec] = { "IGA1 Down Scaling Destination Control", },
		[0xf0] = { "Snapshot Mode - Starting Address of Disp Data", },
		[0xf1] = { "Snapshot Mode - Starting Address of Disp Data", },
		[0xf2] = { "Snapshot Mode - Starting Address of Disp Data", },
		[0xf3] = { "Snapshot Mode Control", },
		[0xf4] = { "Snapshot Mode Control", },
		[0xf5] = { "Snapshot Mode Control", },
		[0xf6] = { "Snapshot Mode Control", },
	},
};

static struct bit_desc seq_19_desc[] = {
	{ 0x01, "CPU Interface Clock Control", },
	{ 0x02, "Display Interface Clock Control", },
	{ 0x04, "MC Interface Clock Control", },
	{ 0x08, "Typical Arbiter Interface Clock Control", },
	{ 0x10, "AGP Interface Clock Control", },
	{ 0x20, "P-Arbiter Interface Clock Control", },
	{ 0x40, "MIU/AGP Interface Clock Control", },
	{ 0 },
};


static struct bit_desc seq_1a_desc[] = {
	{ 0x01, "LUT Shadow Access", },
	{ 0x04, "PCI Burst Write Wait State Select (0: 0 Wait state, 1: 1 Wait state)", },
	{ 0x08, "Extended Mode Memory Access Enable (0: Disable, 1: Enable)", },
	{ 0x40, "Software Reset (0: Default value, 1: Reset)", },
	{ 0x80, "Read Cache Enable (0: Disable, 1: Enable)", },
	{ 0 },
};

static struct bit_desc seq_1b_desc[] = {
	{ 0x01, "Primary Display's LUT Off", },
	{ 0x18, "Primary Display Engine VCK Gating", },
	{ 0x60, "Secondary Display Engine LCK Gating", },
	{ 0 },
};

static struct bit_desc seq_1e_desc[] = {
	{ 0x01, "ROC ECK", },
	{ 0x02, "Replace ECK by MCK", },
	{ 0x08, "Spread Spectrum", },
	{ 0x30, "DVP1 Power Control", },
	{ 0xc0, "VCP Power Control", },
	{ 0 },
};

static struct bit_desc seq_2a_desc[] = {
	{ 0x03, "LVDS Channel 1 Pad Control" },
	{ 0x0c, "LVDS Channel 2 Pad Control" },
	{ 0x40, "Spread Spectrum Type FIFO" },
	{ 0 },
};

static struct bit_desc seq_2b_desc[] = {
	{ 0x01, "MSI Pending IRQ Re-trigger", },
	{ 0x02, "CRT Hot Plug Detect Enable", },
	{ 0x04, "CRT Sense IRQ status", },
	{ 0x08, "CRT Sense IRQ enable", },
	{ 0x10, "LVDS Sense IRQ status", },
	{ 0x20, "LVDS Sense IRQ enable", },
	{ 0 },
};

static struct bit_desc seq_2d_desc[] = {
	{ 0x03, "ECK Pll Power Control", },
	{ 0x0c, "LCK PLL Power Control", },
	{ 0x30, "VCK PLL Power Control", },
	{ 0xc0, "E3_ECK_N Selection", },
	{ 0 },
};

static struct bit_desc seq_2e_desc[] = {
	{ 0x03, "Video Playback Engine V3/V4 Gated Clock VCK", },
	{ 0x0c, "PCI Master / DMA Gated Clock ECK/CPUCK", },
	{ 0x30, "Video Processor Gated Clock ECK", },
	{ 0xc0, "Capturer Gated Clock ECK", },
	{ 0 },
};

static struct bit_desc seq_3c_desc[] = {
	{ 0x01, "AGP Bus Pack Door AGP3 Enable", },
	{ 0x02, "Switch 3 PLLs to Prime Output", },
	{ 0x04, "LCDCK PLL Locked Detect", },
	{ 0x08, "VCK PLL Locked Detect", },
	{ 0x10, "ECL PLL Locked Detect", },
	{ 0x60, "PLL Frequency Division Select for Testing", },
	{ 0 },
};

static struct bit_desc seq_3f_desc[] = {
	{ 0x03, "Video Clock Control (Gated ECK)", },
	{ 0x0c, "2D Clock Control (Gated ECK/CPUCK)", },
	{ 0x30, "3D Clock Control (Gated ECK)", },
	{ 0xc0, "CR Clock Control (Gated ECK)", },
	{ 0 },
};

static struct bit_desc seq_40_desc[] = {
	{ 0x01, "Reset ECK PLL", },
	{ 0x02, "Reset VCK PLL", },
	{ 0x04, "Reset LCDCK PLL", },
	{ 0x08, "LVDS Interrupt Method", },
	{ 0x30, "Free Run ECK Frequency within Idle Mode", },
	{ 0x80, "CRT Sense Enable", },
	{ 0 },
};

static struct bit_desc seq_43_desc[] = {
	{ 0x01, "Notebook Used Flag", },
	{ 0x04, "Typical Channel 1 Arbiter Read Back Data Overwrite Flag", },
	{ 0x08, "Typical Channel 0 Arbiter Read Back Data Overwrite Flag", },
	{ 0x10, "IGA1 Display FIFO Underflow Flag", },
	{ 0x20, "IGA2 Display FIFO Underflow Flag", },
	{ 0x40, "Windows Media Video Enable Flag", },
	{ 0x80, "Advance Video Enable Flag", },
	{ 0 },
};

static struct bit_desc seq_4e_desc[] = {
	{ 0x01, "HQV/Video/Capture Engine Reset", },
	{ 0x02, "HQV/Video/Capture Register Reset", },
	{ 0x04, "2D Engine Reset", },
	{ 0x08, "2D Register Reset", },
	{ 0x10, "3D Engine Reset", },
	{ 0x20, "3D Register Reset", },
	{ 0x40, "CR Engine Reset", },
	{ 0x80, "CR Register Reset", },
	{ 0 },
};

static struct bit_desc seq_59_desc[] = {
	{ 0x01, "GFX-NM AGP Dynamic Clock Enable", },
	{ 0x02, "GFX-NM GMINT Channel 0 Dynamic Clock Enable", },
	{ 0x04, "GFX-NM GMINT Channel 1 Dynamic Clock Enable", },
	{ 0x08, "GFX-NM PCIC Dynamic Clock Enable", },
	{ 0x10, "GFX-NM IGA Dynamic Clock Enable", },
	{ 0x20, "IGA Low Threshold Enable", },
	{ 0x80, "IGA1 Enable", },
	{ 0 },
};

static struct bit_desc seq_5b_desc[] = {
	{ 0x01, "LVDS1 Used IGA2 Source", },
	{ 0x02, "LBDS1 Used IGA1 Source", },
	{ 0x04, "LVDS0 Used IGA2 Source", },
	{ 0x08, "LVDS1 Used IGA1 Source", },
	{ 0x10, "DAC0 Used IGA2 Source", },
	{ 0x20, "DAC0 Used IGA1 Source", },
	{ 0x40, "DAC0 User is TV", },
	{ 0x80, "DCVI Source Selection is TV", },
	{ 0 },
};

static struct bit_desc seq_5c_desc[] = {
	{ 0x01, "DVP1 Used IGA2 Source", },
	{ 0x02, "DVP1 Used IGA1 Source", },
	{ 0x10, "DAC1 Used IGA2 Source", },
	{ 0x20, "DAC1 Used IGA1 Source", },
	{ 0x40, "DAC1 User is TV", },
	{ 0 },
};

static struct bit_desc seq_76_desc[] = {
	{ 0x01, "Backlight Control Enable", },
	{ 0 },
};

struct io_reg sequencer_regs = {
	.io_port_addr = 0x3c4,
	.io_port_data = 0x3c5,
	.name = "Sequencer",
	.index = {
		/* Sequencer Registers */
		[0x00] = { "Reset", },
		[0x01] = { "Clocking Mode", },
		[0x02] = { "Map Mask", },
		[0x03] = { "Character Map Select", },
		[0x04] = { "Memory Mode", },
		/* Extended Sequencer Registers */
		[0x10] = { "Extended Register Unlock", },
		[0x11] = { "Configuration 0", },
		[0x12] = { "Configuration 1", },
		[0x13] = { "Configuration 2 (DVP1 strapping)", },
		[0x14] = { "Frame Buffer Size Control", },
		[0x15] = { "Display Mode Control", },
		[0x16] = { "Display FIFO Threshold Control", },
		[0x17] = { "Display FIFO Control", },
		[0x18] = { "Display Arbiter Control 0", },
		[0x19] = { "Power Management", seq_19_desc, },
		[0x1a] = { "PCI Bus Control", seq_1a_desc, },
		[0x1b] = { "Power Management Control 0", seq_1b_desc, },
		[0x1c] = { "Horizontal Display Fetch Count Data", },
		[0x1d] = { "Horizontal Display Fetch Count Control", },
		[0x1e] = { "Power Management Control", seq_1e_desc, },
		/* 1f: reserved */
		[0x20] = { "Typical Arbiter Control 0", },
		[0x21] = { "Typical Arbiter Control 1", },
		[0x22] = { "Display Arbiter Control 1", },
		[0x26] = { "IIC Serial Port Control 0", },
		[0x2a] = { "Power Management Control 5", seq_2a_desc, },
		[0x2b] = { "LVDS Interrupt Control", seq_2b_desc, },
		[0x2c] = { "General Purpose I/O Port", },
		[0x2d] = { "Power Management Control 1", seq_2d_desc, },
		[0x2e] = { "Power Management Control 2", seq_2e_desc, },
		[0x31] = { "IIC Serial Port Control 1", },
		[0x35] = { "Subsystem Vendor ID Low", },
		[0x36] = { "Subsystem Vendor ID High", },
		[0x37] = { "Subsystem ID Low", },
		[0x38] = { "Subsystem ID High", },
		[0x39] = { "BIOS Reserved Register 0", },
		[0x3a] = { "BIOS Reserved Register 1", },
		[0x3b] = { "PCI Revision ID Back Door", },
		[0x3c] = { "Miscellaneous", seq_3c_desc, },
		[0x3d] = { "General Purpose I/O Port", },
		[0x3e] = { "Miscellaneous Register for AGP Mux", },
		[0x3f] = { "Power Management Control 2", seq_3f_desc, },
		[0x40] = { "PLL Control", seq_40_desc, },
		[0x41] = { "Typical Arbiter Control 1", },
		[0x42] = { "Typical Arbiter Control 2", },
		[0x43] = { "Graphics Bonding Option", seq_43_desc, },
		[0x44] = { "VCK Clock Synthesizer Value 0", },
		[0x45] = { "VCK Clock Synthesizer Value 1", },
		[0x46] = { "VCK Clock Synthesizer Value 2", },
		[0x47] = { "ECK Clock Synthesizer Value 0", },
		[0x48] = { "ECK Clock Synthesizer Value 1", },
		[0x49] = { "ECK Clock Synthesizer Value 2", },
		[0x4a] = { "LDCK Clock Synthesizer Value 0", },
		[0x4b] = { "LDCK Clock Synthesizer Value 1", },
		[0x4c] = { "LDCK Clock Synthesizer Value 2", },
		[0x4d] = { "Preemptive Arbiter Control", },
		[0x4e] = { "Software Reset Control", seq_4e_desc, },
		[0x4f] = { "CR Gating Clock Control", },
		[0x50] = { "AGP Control", },
		[0x51] = { "Display FIFO Control 1", },
		[0x52] = { "Integrated TV Shadow Register Control", },
		[0x53] = { "DAC Sense Control 1", },
		[0x54] = { "DAC Sense Control 2", },
		[0x55] = { "DAC Sense Control 3", },
		[0x56] = { "DAC Sense Control 4", },
		[0x57] = { "Display FIFO Control 2", },
		[0x58] = { "GFX Power Control 1", },
		[0x59] = { "GFX Power Control 2", seq_59_desc, },
		[0x5a] = { "PCI Bus Control 2", },
		[0x5b] = { "Device Used Status 0", seq_5b_desc, },
		[0x5c] = { "Device Used Status 1", seq_5c_desc, },
		[0x5d] = { "Timer Control", },
		[0x5e] = { "DAC Control 2", },
		[0x60] = { "I2C Mode Control", },
		[0x61] = { "I2C Host Address", },
		[0x62] = { "I2C Host Data", },
		[0x63] = { "I2C Host Control", },
		[0x64] = { "I2C Status", },
		[0x65] = { "Power Management Control 6", },
		[0x66] = { "GTI Control 0", },
		[0x67] = { "GTI Control 1", },
		[0x68] = { "GTI Control 2", },
		[0x69] = { "GTI Control 3", },
		[0x6a] = { "GTI Control 4", },
		[0x6b] = { "GTI Control 5", },
		[0x6c] = { "GTI Control 6", },
		[0x6d] = { "GTI Control 7", },
		[0x6e] = { "GTI Control 8", },
		[0x6f] = { "GTI Control 9", },
		[0x70] = { "GARB Control 0", },
		[0x71] = { "Typical Arbiter Control 2", },
		[0x72] = { "Typical Arbiter Control 3", },
		[0x73] = { "Typical Arbiter Control 4", },
		[0x74] = { "Typical Arbiter Control 5", },
		[0x75] = { "Typical Arbiter Control 6", },
		[0x76] = { "Backlight Control 1", seq_76_desc, },
		[0x77] = { "Backlight Control 2", },
		[0x78] = { "Backlight Control 3", },
	},
};

static uint8_t readb_idx_reg(uint16_t port, uint8_t index)
{
	outb(index, port-1);
	return inb(port);
}

static void writeb_idx_reg(uint16_t port, uint8_t index,
			   uint8_t val)
{
	outb(index, port-1);
	outb(val, port);
}

static void writeb_idx_mask(uint16_t reg, uint8_t idx, uint8_t val,
			    uint8_t mask)
{
	uint8_t tmp;

	tmp = readb_idx_reg(reg, idx);
	tmp &= ~ mask;
	tmp |= (val & mask);
	writeb_idx_reg(reg, idx, tmp);
}


struct io_reg *io_regs[] = {
	//&attr_regs,
	&sequencer_regs,
	&graphic_regs,
	&crtc_regs,
	NULL
};

struct half_mode {
	uint16_t total;
	uint16_t active;
	uint16_t blank_start;
	uint16_t blank_end;
	uint16_t retr_start;
	uint16_t retr_end;
	int n_sync;
};

struct mode {
	struct half_mode h;
	struct half_mode v;
	uint32_t addr_start;
	uint8_t bpp;
	uint16_t horiz_quad_count;
	uint16_t horiz_offset;
};

static int get_mode(struct mode *m, int secondary)
{
	uint8_t val;

	memset(m, 0, sizeof(*m));

	if (!secondary) {
		m->h.total	 = readb_idx_reg(0x3d5, 0x00);
		m->h.active	 = readb_idx_reg(0x3d5, 0x01);
		m->h.blank_start = readb_idx_reg(0x3d5, 0x02);
		m->h.blank_end	 = readb_idx_reg(0x3d5, 0x03) & 0x1f;
		m->h.retr_start	 = readb_idx_reg(0x3d5, 0x04);
		m->h.retr_end	 = readb_idx_reg(0x3d5, 0x05) & 0x1f;
		m->v.total	 = readb_idx_reg(0x3d5, 0x06) + 2;

		m->addr_start	 = readb_idx_reg(0x3d5, 0x0d);
		m->addr_start	|= readb_idx_reg(0x3d5, 0x0c) << 8;

		m->v.retr_start	 = readb_idx_reg(0x3d5, 0x10);
		m->v.retr_end	 = readb_idx_reg(0x3d5, 0x11) & 0x0f;
		m->v.active	 = readb_idx_reg(0x3d5, 0x12) + 1;
		m->horiz_offset	 = readb_idx_reg(0x3d5, 0x13);
		m->v.blank_start = readb_idx_reg(0x3d5, 0x15) + 1;
		m->v.blank_end	 = readb_idx_reg(0x3d5, 0x16) + 1;

		/* overflow register 0x07 */
		val = readb_idx_reg(0x3d5, 0x07);
		m->v.total	 |= ((val >> 0) & 0x1) << 8;
		m->v.active	 |= ((val >> 1) & 0x1) << 8;
		m->v.retr_start	 |= ((val >> 2) & 0x1) << 8;
		m->v.blank_start |= ((val >> 3) & 0x1) << 8;
		/* line compare */
		m->v.total	 |= ((val >> 5) & 0x1) << 9;
		m->v.active	 |= ((val >> 6) & 0x1) << 9;
		m->v.retr_start	 |= ((val >> 7) & 0x1) << 9;

		val = readb_idx_reg(0x3d5, 0x09);
		m->v.blank_start |= ((val >> 5) & 0x1) << 9;

		val = readb_idx_reg(0x3d5, 0x33);
		m->h.retr_start	 |= ((val >> 4) & 0x1) << 8;
		m->h.blank_end	 |= ((val >> 5) & 0x1) << 6;

		val = readb_idx_reg(0x3d5, 0x34);
		m->addr_start |= val << 16;

		val = readb_idx_reg(0x3d5, 0x35);
		m->v.total 	 |= ((val >> 0) & 0x1) << 10;
		m->v.retr_start	 |= ((val >> 1) & 0x1) << 10;
		m->v.active	 |= ((val >> 2) & 0x1) << 10;
		m->v.blank_start |= ((val >> 3) & 0x1) << 10;
		//line_comp	 |= ((val >> 4) & 0x1) << 10;
		m->horiz_offset	 |= ((val >> 5) & 0x7) << 8;

		val = readb_idx_reg(0x3d5, 0x36);
		m->h.total	 |= ((val >> 3) & 0x1) << 8;

		val = readb_idx_reg(0x3d5, 0x48);
		m->addr_start	 |= ((val >> 0) & 0x1f) << 24;

		val = readb_idx_reg(0x3c5, 0x15);
		switch ((val >> 2) & 0x3) {
		case 0:
			m->bpp = 8;
			break;
		case 1:
			m->bpp = 16;
			break;
		case 2:
			m->bpp = 30;
			break;
		case 3:
			m->bpp = 32;
			break;
		}

		val = inb(0x3cc);
		if (val & 0x40)
			m->h.n_sync;
		if (val & 0x80)
			m->v.n_sync;

		/* add some weird multipliers and offsets */
		m->h.total = (m->h.total + 5) << 3;
		m->h.active = (m->h.active + 1) << 3;
		m->h.blank_start = (m->h.blank_start + 1) << 3;
		m->h.blank_end = (m->h.blank_end + 1) << 3;
		m->h.retr_start = (m->h.retr_start << 3);
		m->h.retr_end = (m->h.retr_end << 3);

	} else {
		/* horizontal */
		m->h.total	 = readb_idx_reg(0x3d5, 0x50) + 1;
		m->h.active	 = readb_idx_reg(0x3d5, 0x51) + 1;
		m->h.blank_start = readb_idx_reg(0x3d5, 0x52) + 1;
		m->h.blank_end 	 = readb_idx_reg(0x3d5, 0x53) + 1;
		m->h.retr_start	 = readb_idx_reg(0x3d5, 0x56);
		m->h.retr_end	 = readb_idx_reg(0x3d5, 0x57);
		/* add blanking overflow */
		val = readb_idx_reg(0x3d5, 0x54);
		m->h.blank_start |= ((val >> 0) & 0x7) << 8;
		m->h.blank_end	 |= ((val >> 3) & 0x7) << 8;
		m->h.retr_start  |= ((val >> 6) & 0x3) << 8;
		/* add period overflow */
		val = readb_idx_reg(0x3d5, 0x55);
		m->h.total	 |= ((val >> 0) & 0xf) << 8;
		m->h.active	 |= ((val >> 4) & 0x7) << 8;

		/* vertical */
		m->v.total	 = readb_idx_reg(0x3d5, 0x58) + 1;
		m->v.active	 = readb_idx_reg(0x3d5, 0x59) + 1;
		m->v.blank_start = readb_idx_reg(0x3d5, 0x5a) + 1;
		m->v.blank_end	 = readb_idx_reg(0x3d5, 0x5b) + 1;
		m->v.retr_start	 = readb_idx_reg(0x3d5, 0x5e);
		val = readb_idx_reg(0x3d5, 0x5f);
		m->v.retr_end	 = val & 0x1f;
		m->v.retr_start	 |= (val >> 5) << 8;
		/* add blanking overflow */
		val = readb_idx_reg(0x3d5, 0x5c);
		m->v.blank_start |= ((val >> 0) & 0x7) << 8;
		m->v.blank_end	 |= ((val >> 3) & 0x7) << 8;
		m->h.retr_end	 |= ((val >> 6) & 0x1) << 8;
		m->h.retr_start	 |= ((val >> 7) & 0x1) << 10;
		/* add period overflow */
		val = readb_idx_reg(0x3d5, 0x5d);
		m->v.total	 |= ((val >> 0) & 0x7) << 8;
		m->v.active	 |= ((val >> 3) & 0x7) << 8;
		m->h.blank_end	 |= ((val >> 6) & 0x7) << 11;
		m->h.retr_start	 |= ((val >> 7) & 0x7) << 11;

		/* puzzle together the start address */
		val = readb_idx_reg(0x3d5, 0x62);
		m->addr_start = (val >> 1) << 3;
		val = readb_idx_reg(0x3d5, 0x63);
		m->addr_start |= (val << 10);
		val = readb_idx_reg(0x3d5, 0x64);
		m->addr_start |= (val << 18);
		val = readb_idx_reg(0x3d5, 0xa3);
		m->addr_start |= (val & 0x7) << 26;

		m->horiz_quad_count = readb_idx_reg(0x3d5, 0x65);
		m->horiz_offset = readb_idx_reg(0x3d5, 0x66) << 3;

		val = readb_idx_reg(0x3d5, 0x67);
		m->horiz_offset		|= ((val >> 0) & 0x3) << 11;
		m->horiz_quad_count	|= ((val >> 2) & 0x3) << 8;
		switch (val >> 6) {
		case 0:
			m->bpp = 8;
			break;
		case 1:
			m->bpp = 16;
			break;
		case 2:
			m->bpp = 30;
			break;
		case 3:
			m->bpp = 32;
			break;
		}
	}
}

static void dump_scaling(void)
{
	u_int32_t h_scaling, v_scaling;
	uint8_t val;

	val = readb_idx_reg(0x3d5, 0x79);
	if (val & 0x01) {
		printf("Panel Scaling enabled, mode %s\n",
			val & 0x02 ? "Interpolation" : "Duplication");
		v_scaling = (val >> 3) & 0x1;
		h_scaling = ((val >> 4) & 0x3) << 10;
		v_scaling |= ((val >> 6) & 0x3) << 9;

		val = readb_idx_reg(0x3d5, 0x77);
		h_scaling |= val << 2;

		val = readb_idx_reg(0x3d5, 0x78);
		v_scaling |= val << 1;

		val = readb_idx_reg(0x3d5, 0x9f);
		h_scaling |= val & 0x3;
		printf("Scaling Factors: horizontal=%u, vertical=%u\n",
			h_scaling, v_scaling);
	} else
		printf("Panel Scaling disabled\n");
}

static void dump_registers(struct io_reg *ior)
{
	uint8_t idx;

	printf("%s register dump (IO Port address: 0x%03x): \n", ior->name, ior->io_port_addr);
	for (idx = 0; idx < 0xff; idx++) {
		uint8_t val;
		struct bit_desc *desc = ior->index[idx].bit_desc;

		if (!ior->index[idx].name)
			continue;

		outb(idx, ior->io_port_addr);
		val = inb(ior->io_port_data);
		printf("   %03x.%02x = 0x%02x (%s)\n", ior->io_port_data, idx, val,	
			ior->index[idx].name);
		
		if (!desc) 
			continue;

		while (desc->mask) {
			printf("       0x%02x %s: 0x%02x\n", desc->mask,
				desc->name, val & desc->mask);
			desc++;
		}
	}
	printf("\n");
}

enum pll {
	PLL_VCK,
	PLL_ECK,
	PLL_LDCK,
};

#define REF_FREQ	14318

static void get_vck_clock(enum pll pll, unsigned int f_ref_khz)
{
	uint8_t reg_ofs = 0;
	uint8_t val;
	unsigned int dm, dtz, dr, dn;
	unsigned long f_vco, f_out;
	char *name;

	switch (pll) {
	case PLL_VCK:
		reg_ofs = 0;
		name = "VCK";
		break;
	case PLL_ECK:
		reg_ofs = 3;
		name = "ECK";
		break;
	case PLL_LDCK:
		reg_ofs = 6;
		name = "LDCK";
		break;
	default:
		return;
	}

	dm = readb_idx_reg(0x3c5, 0x44 + reg_ofs);

	val = readb_idx_reg(0x3c5, 0x45 + reg_ofs);
	dtz = val & 0x1;
	dr = (val >> 3) & 0x7;
	dm |= ((val >> 6) & 0x3) << 8;

	val = readb_idx_reg(0x3c5, 0x46 + reg_ofs);
	dtz |= (val & 0x1) << 1;
	dn = val >> 1;

	printf("%s PLL: dm=%u, dtx=%u, dr=%u, dn=%u ", name, dm, dtz, dr, dn);

	f_vco = f_ref_khz * (dm + 2) / (dn + 2);
	if (dr)
		f_out = f_ref_khz * (dm + 2) / ( (dn + 2) * (2 * dr) );
	else
		f_out = 0;

	printf("%s Fvco=%lu kHz, Fout=%lu kHz\n", name, f_vco, f_out);
}

struct gpio_state {
	u_int32_t mode_output;
	u_int32_t pin_status;
	u_int32_t output_bit;
	u_int32_t alt_function;
};

static int get_gpio_state(struct gpio_state *s)
{
	uint8_t val;

	memset(s, 0, sizeof(*s));

	val = readb_idx_reg(0x3c5, 0x2c);
	if (val & 0x01)
		s->alt_function |= (3 << 2);
	if (val & 0x04)
		s->pin_status |= (1 << 3);
	if (val & 0x08)
		s->pin_status |= (1 << 2);
	if (val & 0x10)
		s->output_bit |= (1 << 3);
	if (val & 0x20)
		s->output_bit |= (1 << 2);
	if (val & 0x40)
		s->mode_output |= (1 << 3);
	if (val & 0x80)
		s->mode_output |= (1 << 2);

	val = readb_idx_reg(0x3c5, 0x3d);
	if (val & 0x01)
		s->alt_function |= (3 << 4);
	if (val & 0x04)
		s->pin_status |= (1 << 5);
	if (val & 0x08)
		s->pin_status |= (1 << 4);
	if (val & 0x10)
		s->output_bit |= (1 << 5);
	if (val & 0x20)
		s->output_bit |= (1 << 4);
	if (val & 0x40)
		s->mode_output |= (1 << 5);
	if (val & 0x80)
		s->mode_output |= (1 << 4);

}

static void dump_gpio_state(const char *pfx, const struct gpio_state *gs)
{
	unsigned int i;

	for (i = 2; i < 6; i++) {
		printf("%sGPIO %u: function=", pfx, i);

		if (gs->alt_function & (1 << i))
			printf("alternate\n");
		else if (gs->mode_output & (1 << i))
			printf("output(%u)\n", gs->output_bit & (1 <<i) ? 1 : 0);
		else
			printf("input(%u)\n", gs->pin_status & (1 << i) ? 1 : 0);
	}
}

static void dump_all_registers(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(io_regs); i++) {
		struct io_reg *reg = io_regs[i];
		if (!reg)
			break;
		dump_registers(reg);
	}

}

static void dump_mode(const char *pfx, struct mode *m)
{
	printf("%sH total=%u, active=%u, blank (%u-%u), sync(%u-%u)\n",
		pfx, m->h.total, m->h.active, m->h.blank_start, m->h.blank_end,
		m->h.retr_start, m->h.retr_end);
	printf("%sV total=%u, active=%u, blank (%u-%u), sync(%u-%u)\n",
		pfx, m->v.total, m->v.active, m->v.blank_start, m->v.blank_end,
		m->v.retr_start, m->v.retr_end);
	printf("base_addr=0x%08x, bpp=%d\n", m->addr_start, m->bpp);
}

static void dump_sl(const char *pfx)
{
	uint8_t val;
	unsigned int sl_size_mb;
	unsigned long rtsf_in_sl_addr;
	uint64_t sl_in_mem_addr, temp;

	val = readb_idx_reg(0x3c5, 0x68);
	switch (val) {
	case 0:
		sl_size_mb = 512;
		break;
	case 0x80:
		sl_size_mb = 256;
		break;
	case 0xc0:
		sl_size_mb = 128;
		break;
	case 0xe0:
		sl_size_mb = 64;
		break;
	case 0xf0:
		sl_size_mb = 32;
		break;
	case 0xf8:
		sl_size_mb = 16;
		break;
	case 0xfc:
		sl_size_mb = 8;
		break;
	case 0xfe:
		sl_size_mb = 4;
		break;
	case 0xff:
		sl_size_mb = 2;
		break;
	}

	rtsf_in_sl_addr  = readb_idx_reg(0x3c5, 0x6a) << 12;
	rtsf_in_sl_addr |= readb_idx_reg(0x3c5, 0x6b) << 20;
	val = readb_idx_reg(0x3c5, 0x6c);
	rtsf_in_sl_addr |= (val & 0x1) << 28;

	sl_in_mem_addr = readb_idx_reg(0x3c5, 0x6d) << 21;
	sl_in_mem_addr |= readb_idx_reg(0x3c5, 0x6d) << 29;
	temp = (readb_idx_reg(0x3c5, 0x6d) & 0x7f);
	sl_in_mem_addr |= (temp << 37);

	printf("%sSL in System memory: 0x%llx, RTSF in SL: 0x%lx\n",
		pfx, sl_in_mem_addr, rtsf_in_sl_addr);
}

static int dump_lvds(void)
{
	uint8_t val;
	char *mode;

	writeb_idx_mask(0x3c5, 0x5a, 0x01, 0x01);

	val = readb_idx_reg(0x3c5, 0x13);
	switch (val >> 6) {
	case 0:
		mode = "LVDS1 + LVDS2";
		break;
	case 2:
		mode = "One Dual LVDS Channel";
		break;
	default:
		mode = "RESERVED";
		break;
	}
	printf("LVDS Seq Mode: %s\n", mode);

	val = readb_idx_reg(0x3d5, 0xd2);
	switch ((val >> 4) & 3) {
	case 0:
		mode = "LVDS1 + LVDS2";
		break;
	case 2:
		mode = "One Dual LVDS Channel";
		break;
	default:
		mode = "RESERVED";
		break;
	}
	printf("LVDS CRT Mode: %s\n", mode);
	printf("LVDS Channel 1 Format %s, Power %s\n",
		val & 2 ? "OpenLDI":"SPWG", val & 0x80 ? "Down" : "Up");
	printf("LVDS Channel 2 Format %s, Power %s\n",
		val & 1 ? "OpenLDI":"SPWG", val & 0x40 ? "Down" : "Up");
	
}
static int parse_ioreg(uint16_t *reg, uint8_t *index, char *str)
{
	char *dot;
	char buf[255];

	memset(buf, 0, sizeof(*buf));
	strncpy(buf, str, sizeof(buf)-1);

	dot = strchr(buf, '.');
	if (!dot)
		return -EINVAL;
	*dot = '\0';

	*reg = strtoul(buf, NULL, 16);
	*index = strtoul(dot+1, NULL, 16);
	
	return 0;
}

static void reset_mode(int secondary)
{
	if (!secondary) {
		writeb_idx_mask(0x3d5, 0x11, 0x00 , 0x80);
		writeb_idx_mask(0x3d5, 0x03, 0x80 , 0x80);
	} else {
	}
}

static void unlock_registers(void)
{
	writeb_idx_reg(0x3c5, 0x10, 0x01); /* unlock extended */
	writeb_idx_mask(0x3d5, 0x47, 0x00, 0x01);	/* unlock CRT */
	writeb_idx_mask(0x3d5, 0x03, 0x80, 0x80);	/* disable EGA lightpen */
	writeb_idx_mask(0x3d5, 0x11, 0x00, 0x80);	/* unlock 0..7 */
}

static void usage(void)
{
	printf("Usage :\n");
	printf("-h | --help  : Display this usage message.\n");
	printf("-d | --dump  : Dump all registers.\n");
	printf("-p | --pll   : Display PLL.\n");
	printf("-m | --mode  : Display modes.\n");
	printf("-r | --read  : Read register. Example : $0 -r 3d5.17\n");
	printf("-w | --write : Write register. Example : $0 -w 3d5.17 0xa3\n");
	printf("-g | --gpio  : Display GPIO state.\n");
}

int main(int argc, char **argv)
{
	struct mode m;
	struct gpio_state gs;
	int rc, option_index = 0;

	printf("via-chrome-tool (C) 2009 by VIA Technologies, Inc.\n");
	printf("This is FREE SOFTWARE with ABSOLUTELY NO WARRANTY\n\n");

	rc = iopl(3);
	if (rc < 0) {
		perror("iopl");
		printf("Need root privileges.\n");
		exit(1);
	}

	if (argc <= 1) {
		usage();
		exit(1);
	}

	unlock_registers();

	while (1) {
		int c;
		uint16_t reg;
		uint8_t index;
		unsigned long val;
		static struct option long_options[] = {
			{ "help", 0, 0, 'h' },
			{ "dump", 0, 0, 'd' },
			{ "pll", 0, 0, 'p' },
			{ "mode", 0, 0, 'm' },
			{ "read", 1, 0, 'r' },
			{ "write", 1, 0, 'w' },
			{ "gpio", 1, 0, 'g' },
		};

		c = getopt_long(argc, argv, "hdpmr:w:g", long_options,
				&option_index);

		if (c == -1) {
			break;
		}

		switch (c) {
		case 'h':
			usage();
			exit(1);
		case 'd':
			dump_all_registers();
			break;
		case 'p':
			get_vck_clock(PLL_VCK, REF_FREQ);
			get_vck_clock(PLL_ECK, REF_FREQ);
			get_vck_clock(PLL_LDCK, REF_FREQ);
			break;
		case 'm':
			dump_sl("");
			printf("Primary Display:\n");
			get_mode(&m, 0);
			dump_mode("    ", &m);
			printf("\n");
			printf("Secondary Display:\n");
			get_mode(&m, 1);
			dump_mode("    ", &m);
			printf("\n");
			dump_scaling();
			printf("\n");
			dump_lvds();
			printf("\n");
			break;
		case 'r':
			parse_ioreg(&reg, &index, optarg);
			printf("%03x.%02x = 0x%02x\n", reg, index,
				readb_idx_reg(reg, index));
			break;
		case 'w':
			parse_ioreg(&reg, &index, optarg);
			/* we need one extra argument */
			if (argc <= optind)
				exit(1);
			val = strtoul(argv[optind], NULL, 16);
			if (val > 0xff)
				exit(1);
			writeb_idx_reg(reg, index, val);
			printf("%03x.%02x = 0x%02x\n", reg, index,
				readb_idx_reg(reg, index));
			break;
		case 'g':
			printf("GPIO State\n");
			get_gpio_state(&gs);
			dump_gpio_state("    ", &gs);
			printf("\n");
			break;
		default:
			usage();
			exit(1);
		}
	}

	exit(0);
}
