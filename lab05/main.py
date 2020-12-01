#!/usr/bin/env python3
# coding: utf-8

import argparse
import smtplib
import os
from sys import stdin
from email.message import EmailMessage
from typing import Tuple


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description='SMTP client.')
    parser.add_argument('to', help='Set address of the recipient')
    parser.add_argument('from', help='Set sender address')
    parser.add_argument('password')
    parser.add_argument('--keyword', help='Attach text files including the keyword from client\'s folder')
    parser.add_argument('--folder', help='Set client folder', default='.')
    return parser.parse_args()


def attach_files(msg: EmailMessage, folder: str, keyword: str) -> Tuple[int, int]:
    n_txt_files = 0
    n_attached = 0
    for root, dirnames, filenames in os.walk(folder):
        for filename in filenames:
            if filename.endswith('.txt'):
                n_txt_files += 1
                with open(os.path.join(root, filename)) as file:
                    content = file.read()
                    if keyword in content:
                        n_attached += 1
                        msg.add_attachment(content, filename=filename)
    return n_txt_files, n_attached


def main() -> None:
    args = vars(parse_args())

    msg = EmailMessage()
    msg['From'] = args['from']
    msg['To'] = args['to']
    msg['Subject'] = input('Subject: ')

    print('\nBody (ending with EOF):')
    msg.set_content(stdin.read())

    if args['keyword'] is not None:
        n_txt_files, n_attached = attach_files(msg, args['folder'], args['keyword'])
        print(f'\nText files in {args["folder"]} found: {n_txt_files}')
        print(f'Attached: {n_attached}')

    server = smtplib.SMTP_SSL('smtp.yandex.ru:465')
    server.login(args['from'], args['password'])
    server.send_message(msg)


if __name__ == '__main__':
    main()
