#!/usr/bin/env python3
# -*- coding:utf-8 -*-
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
"""
Author  :   xuechengyun
E-mail  :   xuechengyunxue@gmail.com
Date    :   24/09/27 15:53:46
Desc    :
"""

import sys
import os
import logging


CUR_DIR = os.path.dirname(os.path.abspath(__file__))
LOG_FORMAT = '''[%(levelname)s] [%(asctime)s] [%(threadName)s] [%(name)s] '''
LOG_FORMAT += '''[%(filename)s:%(funcName)s:%(lineno)d]: %(message)s'''
logging.basicConfig(
    stream=sys.stderr,
    level=logging.INFO,
    format=LOG_FORMAT
)

import glob
import subprocess

def visualize_one(source, target):
    logging.info(f"start visualize, source {source}, target {target}")
    if not os.path.exists(source):
        logging.warning(f"source not exist, filepath {source}")
        return
    subprocess.run([f'dot -Tsvg {source} -o {target}'], shell=True)


def visualize(files):
    for file in files:
        pre, _ = os.path.splitext(file)
        output = pre + '.svg'
        visualize_one(file, output)


def get_all_files():
    for file in glob.iglob("**/*.dot", recursive=True):
        yield file


def main():
    if len(sys.argv) < 2:
        visualize(get_all_files())
    else:
        visualize(sys.argv[1:])

if __name__ == '__main__':
    main()

