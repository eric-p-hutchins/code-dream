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

(let ((filename (car command-line-args-left)))
  (find-file filename)
  (font-lock-fontify-buffer)

  ;; For each character, get the face and print it after the character
  (dotimes (i (buffer-size))
    (let* ((face (get-text-property (1+ i) 'face))
           (face-name (if (null face)
                          nil
                        ;; If there's a list of faces, just take the
                        ;; first one
                        (if (listp face)
                            (face-name (car face))
                          (face-name face)))))
      (princ (buffer-substring (1+ i) (+ 2 i)))
      (princ " ")
      (princ face-name)
      (princ "\n")
    )))
