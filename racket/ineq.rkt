#lang racket
(require rackunit "intvl.rkt")
(provide (all-defined-out))


;; (solve-ineq const coeff op target) produces an interval containing
;;   the solutions to the inequality [const] + [coeff]n [op] [target].
;; solve-ineq: Num Num (Num Num -> Bool) Num -> Interval
;; Requires: coeff != 0
(define (solve-ineq const coeff op target)
  (local
    [(define bound (/ (- target const) coeff))
     (define op>=? (or (equal? op >) (equal? op >=)))
     (define open? (or (equal? op >) (equal? op <)))]

    (cond [(= target +inf)
           (cond [op>=? false]
                 [else real])]
          [(= target -inf)
           (cond [op>=? real]
                 [else false])]
          [(positive? coeff)
           (cond [op>=? (make-interval bound +inf open? true)]
                 [else (make-interval -inf bound true open?)])]
          [(negative? coeff)
           (cond [op>=? (make-interval -inf bound true open?)]
                 [else (make-interval bound +inf open? true)])])))

;; Tests:
(check-equal? (solve-ineq -137 5 > 0) (make-interval 137/5 +inf true true))
(check-equal? (solve-ineq 274 -9 > 0) (make-interval -inf 274/9 true true))
(check-equal? (solve-ineq 137 5 > 0) (make-interval -137/5 +inf true true))
(check-equal? (solve-ineq 274 9 > 0) (make-interval -274/9 +inf true true))
(check-equal? (solve-ineq -137 -5 > 0) (make-interval -inf -137/5 true true))
(check-equal? (solve-ineq -274 -9 > 0) (make-interval -inf -274/9 true true))
(check-equal? (solve-ineq 137 -5 > 0) (make-interval -inf 137/5 true true))
(check-equal? (solve-ineq -274 9 > 0) (make-interval 274/9 +inf true true))
(check-equal? (solve-ineq 50 4 >= 0) (make-interval -50/4 +inf false true))
(check-equal? (solve-ineq -50 -5 >= 0) (make-interval -inf -10 true false))
(check-equal? (solve-ineq 4 7 < 6) (make-interval -inf 2/7 true true))
(check-equal? (solve-ineq 5 -3 < 8) (make-interval -1 +inf true true))
(check-equal? (solve-ineq 4 7 <= 6) (make-interval -inf 2/7 true false))
(check-equal? (solve-ineq 5 -3 <= 8) (make-interval -1 +inf false true))
(check-equal? (solve-ineq 0 -1 <= 0) nonneg)
(check-equal? (solve-ineq 50 4 < +inf) real)


;; (solve-ineq-in const coeff intvl) produces an interval containing
;;   the solutions to the expression [const] + [coeff]n, where n is
;;   in the interval intvl. 
;; solve-ineq-in: Num Num Interval -> Interval
;; Requires: coeff != 0
(define (solve-ineq-in const coeff intvl)
  (cond [(not (valid-interval? intvl)) false]
        [else
         (local
           [(define intvl>=
              (solve-ineq const coeff
                          (cond [(interval-left-open? intvl) >]
                                [else >=])
                          (interval-low intvl)))
            (define intvl<=
              (solve-ineq const coeff
                          (cond [(interval-right-open? intvl) <]
                                [else <=])
                          (interval-high intvl)))]

           (intersection intvl>= intvl<=))]))

;; Tests:
(check-equal? (solve-ineq-in 50 4 pos)
              (make-interval -50/4 +inf true true))
(check-equal? (solve-ineq-in 50 4 neg)
              (make-interval -inf -50/4 true true))
(check-equal? (solve-ineq-in 50 -4 pos)
              (make-interval -inf 50/4 true true))
(check-equal? (solve-ineq-in -50 4 pos)
              (make-interval 50/4 +inf true true))
(check-equal? (solve-ineq-in -50 -4 pos)
              (make-interval -inf -50/4 true true))


;; (solve-ineq-sys x-pair y-pair xi yi) produces an interval containing
;;   the solutions to the system of inequality given by x-pair and y-pair,
;;   where x is in the interval xi, and y is in the interval yi.
;; solve-ineq-sys: (list Num Num) (list Num Num) Interval Interval -> Interval
(define (solve-ineq-sys x-pair y-pair xi yi)
  (intersection (solve-ineq-in (first x-pair) (second x-pair) xi)
                (solve-ineq-in (first y-pair) (second y-pair) yi)))

;; Tests:
(check-equal? (solve-ineq-sys '(-137 5) '(274 -9) pos pos)
              (make-interval 137/5 274/9 true true))
(check-equal? (solve-ineq-sys '(137 5) '(274 9) pos pos)
              (make-interval -137/5 +inf true true))
(check-equal? (solve-ineq-sys '(-137 -5) '(-274 -9) pos pos)
              (make-interval -inf -274/9 true true))
(check-equal? (solve-ineq-sys '(137 -5) '(-274 9) pos pos)
              false)
(check-equal? (solve-ineq-sys '(50 4) '(-50 -5) pos pos)
              (make-interval -50/4 -10 true true))
(check-equal? (solve-ineq-sys '(-50 4) '(-50 5) pos pos)
              (make-interval 50/4 +inf true true))
