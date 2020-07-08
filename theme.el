#!/usr/bin/env emacs --script
;; Copyright (C) 2020 Eric P. Hutchins

;; This file is part of Code Dream.

;; Code Dream is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; Code Dream is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with Code Dream.  If not, see <https:;;www.gnu.org/licenses/>.
(load "~/.emacs")
(require 'color)
(require 'seq)
(autoload-do-load 'color-rgb-to-hex)

(setq theme-name nil)
(setq background 'light)
(setq case-fold-search t)

(defun face-spec-display-attribute (spec attribute)
  (if (eq (car spec) 't)
      nil
    (let* ((display (car spec))
           (val (seq-find (lambda (x)
                            (if (eq (car x) attribute)
                                (cadr x)))
                          display)))
      (if val (cadr val)))))

(defun face-spec-class (spec)
  (face-spec-display-attribute spec 'class))

(defun face-spec-min-colors (spec)
  (face-spec-display-attribute spec 'min-colors))

(defun face-spec-background (spec)
  (face-spec-display-attribute spec 'background))

(defun better-spec (spec best-spec)
  (let ((spec-background (face-spec-background spec))
        (class (face-spec-class spec))
        (min-colors (face-spec-min-colors spec)))
    ;; background has to not be specified or match what we are trying
    ;; to get
    (and (or (null spec-background) (eq spec-background background))
         ;; Either there is not any spec yet
         (or (null best-spec)
             ;; min-colors has to be better than what we already have
             (null (face-spec-min-colors best-spec))
             ;; if we already have min-colors, then this spec's
             ;; min-colors has to be non-null and greater
             (and (not (null min-colors))
                  (> min-colors (face-spec-min-colors best-spec)))))))

(defun pick-face-spec (face-specs)
  (let ((best-spec nil))
    (dolist (spec face-specs)
      (let ((display (car spec)))
        (if (better-spec spec best-spec)
            (setq best-spec spec))))
    best-spec))

(defun face-theme-attributes (theme-name face-name)
  (let* ((theme-face (cdr (assq theme-name (get face-name 'theme-face))))
         (face-specs (car theme-face))
         (face-specs-alist (mapcar (lambda (x) (cons (car x) (cadr x))) face-specs))
         (face-spec (pick-face-spec face-specs-alist)))
    (cdr face-spec)))

(defun color-alist-name (name)
  (replace-regexp-in-string " " "" (downcase name)))

(defun color-name-to-hex (name)
  ;; If it's already a 6 digit hex, then just return it
  (cond ((string-match "^#[0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f]$" name)
         name)
        ((string-match "^#[0-9a-f][0-9a-f][0-9a-f]$" name)
         (apply 'color-rgb-to-hex
                (mapcar (lambda (x)
                          (/ (float (string-to-number (string x) 16))
                             15))
                        (cdr (string-to-list name)))))
        (t (let ((color (assoc (color-alist-name name) color-name-rgb-alist)))
             (if color
                 (let ((max-value (car (color-values "#ffffff")))
                       (value (cdr color)))
                   (apply 'color-rgb-to-hex
                          (append (mapcar (lambda (x)
                                            (/ x (float max-value)))
                                          value)
                                  '(2))))
               (if (eq background 'light)
                   "#000000"
                 "#ffffff"))))))

(defun get-background (theme-name)
  (let* ((attributes (face-theme-attributes theme-name 'default))
         (background (plist-get attributes :background)))
    (if background
        (color-name-to-hex background)
      (if (eq background 'light)
          "#ffffff"
        "#000000"))))

(defun get-face-foreground (theme-name face-name)
  (let* ((attributes (face-theme-attributes theme-name face-name))
         (foreground (plist-get attributes :foreground)))
    (if foreground
        (color-name-to-hex foreground)
      (if (eq background 'light)
          "#000000"
        "#ffffff"))))

(defun get-face-weight (theme-name face-name)
  (let* ((attributes (face-theme-attributes theme-name face-name))
         (weight (plist-get attributes :weight)))
    (if (memq weight '(semi-bold bold extra-bold ultra-bold))
        "B"
      "N")))

(defun write-background-color (theme-name)
  (let ((background (get-background theme-name)))
    (if (not (null background))
        (princ (substring background 1))
      (princ "000000"))))

(defun write-face (theme-name face-name)
  (let ((color (get-face-foreground theme-name face-name))
        (weight (get-face-weight theme-name face-name)))
    (if color
        (princ (substring color 1))
      (princ "ffffff"))
    (princ weight)))

(defun get-default-foreground-color (face)
  (let* ((face-specs (get face 'face-defface-spec))
         (face-spec (pick-face-spec face-specs))
         (attributes (cdr face-spec))
         (foreground (plist-get attributes :foreground)))
    (if foreground
        (color-name-to-hex foreground)
      (if (eq background 'light)
          "#000000"
        "#ffffff"))))

(defun get-default-weight (face)
  (let* ((face-specs (get face 'face-defface-spec))
         (face-spec (pick-face-spec face-specs))
         (attributes (cdr face-spec))
         (weight (plist-get attributes :weight)))
    (if (memq weight '(semi-bold bold extra-bold ultra-bold))
        "B"
      "N")))

(defun write-default (face-name)
  (let ((color (get-default-foreground-color face-name))
        (weight (get-default-weight face-name)))
    (if color
        (princ (substring color 1))
      (if (eq background 'light)
          (princ "000000")
        (princ "ffffff")))
    (princ weight)))

(defun write-defaults ()
  (if (eq background 'light)
      (princ "ffffff")
    (princ "000000"))
  (write-default 'default)
  (write-default 'font-lock-builtin-face)
  (write-default 'font-lock-string-face)
  (write-default 'font-lock-keyword-face)
  (write-default 'font-lock-type-face)
  (write-default 'font-lock-function-name-face)
  (write-default 'font-lock-variable-name-face)
  (write-default 'font-lock-constant-face)
  (write-default 'font-lock-comment-face))

(defun write-theme (theme)
  (write-background-color theme)
  (write-face theme 'default)
  (write-face theme 'font-lock-builtin-face)
  (write-face theme 'font-lock-string-face)
  (write-face theme 'font-lock-keyword-face)
  (write-face theme 'font-lock-type-face)
  (write-face theme 'font-lock-function-name-face)
  (write-face theme 'font-lock-variable-name-face)
  (write-face theme 'font-lock-constant-face)
  (write-face theme 'font-lock-comment-face))

(defun theme-exists-in (theme path)
  (cond
   ((eq 'custom-theme-directory path)
      (progn
        (file-exists-p (concat custom-theme-directory (symbol-name theme) "-theme.el"))))
   ((eq 't path)
      (progn
        (file-exists-p (concat data-directory "themes/" (symbol-name theme) "-theme.el"))))
   (t (progn
        (file-exists-p (concat path (symbol-name theme) "-theme.el"))))))

(defun my-or (&rest list)
  (if (null list)
      nil
    (if (car list)
        t
      (apply 'my-or (cdr list)))))

(defun theme-exists (theme)
  (apply 'my-or (mapcar (lambda (x)
                          (theme-exists-in theme x))
                        custom-theme-load-path)))

(dolist (arg command-line-args-left)
  (if (string= arg "--dark")
      (setq background 'dark)
    (setq theme-name arg)))

(if (null theme-name)
    (write-defaults)
  (let* ((theme (intern theme-name)))
    (if (theme-exists theme)
        (progn
          (load-theme theme)
          (write-theme theme))
      (progn
        (write-defaults)))))

(setq argv nil)
