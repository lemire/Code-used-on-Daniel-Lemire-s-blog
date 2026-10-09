# A toy command-line tool: many imports at the top, most of them only
# needed by one subcommand.
import argparse
import sys
import json
import csv
import decimal
import sqlite3
import asyncio
import email.parser
import http.client
import urllib.request
import xml.etree.ElementTree
import zipfile
import tarfile
import statistics
import numpy as np
import pandas as pd
import requests
import rich.console

__version__ = "1.0"


def cmd_stats(path):
    df = pd.read_csv(path)
    print(df.describe().loc["mean"].to_string())


def cmd_mean(path):
    with open(path, newline="") as f:
        rows = list(csv.reader(f))[1:]
    print(statistics.fmean(float(r[0]) for r in rows))


def main():
    parser = argparse.ArgumentParser(prog="tool")
    parser.add_argument("--version", action="store_true")
    parser.add_argument("command", nargs="?")
    parser.add_argument("path", nargs="?")
    args = parser.parse_args()
    if args.version:
        print(__version__)
    elif args.command == "stats":
        cmd_stats(args.path)
    elif args.command == "mean":
        cmd_mean(args.path)


main()
