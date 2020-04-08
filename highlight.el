#!/usr/bin/env emacs --script
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
