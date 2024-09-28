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

def get_all_dot_files(root_dir):
    for file in glob.iglob("**/*.dot", root_dir=root_dir, recursive=True):
        yield os.path.join(root_dir, file)

def visualize(files):
    for file in files:
        pre, _ = os.path.splitext(file)
        output = pre + '.svg'
        visualize_one(file, output)

def visualize_all(paths):
    for path in paths:
        if os.path.isdir(path):
            visualize(get_all_dot_files(path))
        else:
            visualize([path])


def main():
    if len(sys.argv) < 2:
        visualize_all("task_graph")
    else:
        visualize_all(sys.argv[1:])

if __name__ == '__main__':
    main()

