
(cl:in-package :asdf)

(defsystem "koenig-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "custom" :depends-on ("_package_custom"))
    (:file "_package_custom" :depends-on ("_package"))
  ))