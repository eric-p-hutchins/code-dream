#!/usr/bin/env emacs --script
(let ((filename (car command-line-args-left)))
  (find-file filename)
  (font-lock-fontify-buffer)

  ;; For each character, get the face and store it or print it somehow
  (dotimes (i (buffer-size))
    (let* ((face (get-text-property (1+ i) 'face))
           (face-name (if face (face-name face))))
      (princ (buffer-substring (1+ i) (+ 2 i)))
      (princ " ")
      (princ face-name)
      (princ "\n")
    )))
