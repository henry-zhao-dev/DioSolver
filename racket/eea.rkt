#lang racket
(require rackunit)
(provide (all-defined-out))


(define-struct eear (x y r q) #:transparent)
;; An Extended Euclidean Algorithm Row (EEAR) is a
;;   (make-eear Int Int Nat Nat)


;; (eea-table a b) produces a full EEA table of a and b.
;; eea-table: Int Int -> (listof EEAR)
(define (eea-table a b)
  (local
    [;; First two rows
     (define r1 (make-eear 1 0 (max (abs a) (abs b)) 0))
     (define r2 (make-eear 0 1 (min (abs a) (abs b)) 0))

     ;; (apply-eea/lst r1 r2) produces the completed EEA table of a and b
     ;;   based on the previous two rows, r1 and r2.
     ;; apply-eea/lst: EEAR EEAR -> (listof EEAR)
     (define (apply-eea/lst r1 r2)
       (cond [(zero? (eear-r r2)) empty]
             [else
              (local
                [(define q (quotient (eear-r r1) (eear-r r2)))
                 (define r (remainder (eear-r r1) (eear-r r2)))
                 (define x (- (eear-x r1) (* (eear-x r2) q)))
                 (define y (- (eear-y r1) (* (eear-y r2) q)))
                 (define eear (make-eear x y r q))]

                (cons eear (apply-eea/lst r2 eear)))]))]
    
    (append (list r1 r2) (apply-eea/lst r1 r2))))

;; Tests:
(check-equal? (eea-table 0 5)
              (list (make-eear 1 0 5 0)
                    (make-eear 0 1 0 0)))

(check-equal? (eea-table -5 -1)
              (list (make-eear 1 0 5 0)
                    (make-eear 0 1 1 0)
                    (make-eear 1 -5 0 5)))

(check-equal? (eea-table 5 5)
              (list (make-eear 1 0 5 0)
                    (make-eear 0 1 5 0)
                    (make-eear 1 -1 0 1)))

(check-equal? (eea-table 5 -7)
              (list (make-eear 1 0 7 0)
                    (make-eear 0 1 5 0)
                    (make-eear 1 -1 2 1)
                    (make-eear -2 3 1 2)
                    (make-eear 5 -7 0 2)))

(check-equal? (eea-table 1386 322)
              (list (make-eear 1 0 1386 0)
                    (make-eear 0 1 322 0)
                    (make-eear 1 -4 98 4)
                    (make-eear -3 13 28 3)
                    (make-eear 10 -43 14 3)
                    (make-eear -23 99 0 2)))

(check-equal? (eea-table -2172 423)
              (list (make-eear 1 0 2172 0)
                    (make-eear 0 1 423 0)
                    (make-eear 1 -5 57 5)
                    (make-eear -7 36 24 7)
                    (make-eear 15 -77 9 2)
                    (make-eear -37 190 6 2)
                    (make-eear 52 -267 3 1)
                    (make-eear -141 724 0 2)))


;; (eea-2nd-last-row a b) produces the second last row of the EEA table
;;   of a and b.
;; eea-2nd-last-row: Int Int -> EEAR
(define (eea-2nd-last-row a b)
  (local
    [;; (apply-eea r1 r2) produces the second last row of the EEA table
     ;;   of a and b based on the previous two rows, r1 and r2.
     ;; apply-eea: EEAR EEAR -> EEAR
     (define (apply-eea r1 r2)
       (cond [(zero? (eear-r r2)) r1]
             [else
              (local
                [(define q (quotient (eear-r r1) (eear-r r2)))
                 (define r (remainder (eear-r r1) (eear-r r2)))
                 (define x (- (eear-x r1) (* (eear-x r2) q)))
                 (define y (- (eear-y r1) (* (eear-y r2) q)))
                 (define eear (make-eear x y r q))]
                
                (apply-eea r2 eear))]))]

    (apply-eea (make-eear 1 0 (max (abs a) (abs b)) 0)
               (make-eear 0 1 (min (abs a) (abs b)) 0))))

;; Tests:
(check-equal? (eea-2nd-last-row 0 5) (make-eear 1 0 5 0))
(check-equal? (eea-2nd-last-row -5 -1) (make-eear 0 1 1 0))
(check-equal? (eea-2nd-last-row 5 5) (make-eear 0 1 5 0))
(check-equal? (eea-2nd-last-row 5 -7) (make-eear -2 3 1 2))
(check-equal? (eea-2nd-last-row 1386 322) (make-eear 10 -43 14 3))
(check-equal? (eea-2nd-last-row -2172 423) (make-eear 52 -267 3 1))


(define eea-gcd
  (case-lambda
    ;; (eea-gcd a b) produces the greatest common divisor of a and b.
    ;; eea-gcd: Int Int -> Nat
    [(a b) (eear-r (eea-2nd-last-row a b))]
    
    ;; (eea-gcd row) produces the greatest common divisor based on row.
    ;; eea-gcd: EEAR -> Nat
    ;; Requires: row is the second last row of an EEA table
    [(row) (eear-r row)]))

;; Tests:
(check-equal? (eea-gcd 0 5) 5)
(check-equal? (eea-gcd -5 0) 5)
(check-equal? (eea-gcd -5 -1) 1)
(check-equal? (eea-gcd 5 5) 5)
(check-equal? (eea-gcd 5 -7) 1)
(check-equal? (eea-gcd 1386 322) 14)
(check-equal? (eea-gcd -2172 423) 3)
(check-equal? (eea-gcd (make-eear 10 -43 14 3)) 14)
(check-equal? (eea-gcd (make-eear 52 -267 3 1)) 3)
