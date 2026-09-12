#lang racket
(require rackunit "eea.rkt" "intvl.rkt" "ineq.rkt")


(define max-result 10)
(define var "n")
(define no-soln "No Solution!")


;; (eea-lde a b c [row]) produces a pair of a particular solution
;;   to the linear Diophantine equation, ax + by = c, or false if
;;   no solution exists; #:row may be explicitly provided to avoid
;;   computing the second last row of the EEA table of a and b.
;; eea-lde: Int Int Int [EEAR] -> (anyof false (list Int Int))
;; Requires: row is the second last row of the EEA table of a and b.
(define (eea-lde a b c #:row [row (eea-2nd-last-row a b)])
  (local
    [(define gcd-ab (eea-gcd row))]

    (cond
      ;; By LDET2 (MATH 135, UWaterloo), if gcd(a,b) does not divide c,
      ;;   then there is no solution to the linear Diophantine equation,
      ;;   ax + by = c.
      [(not (zero? (remainder c gcd-ab))) false]
      [else
       (local
         [(define factor (/ c gcd-ab))
          (define x (* (eear-x row) factor))
          (define y (* (eear-y row) factor))]

         (cond [(> (abs a) (abs b))
                (list (* x (/ (abs a) a)) (* y (/ (abs b) b)))]
               [else
                (list (* y (/ (abs a) a)) (* x (/ (abs b) b)))]))])))

;; Tests:
(check-equal? (eea-lde 2172 423 3) (list 52 -267))
(check-equal? (eea-lde -2172 423 3) (list -52 -267))
(check-equal? (eea-lde 2172 -423 3) (list 52 267))
(check-equal? (eea-lde -2172 -423 3) (list -52 267))
(check-equal? (eea-lde -2172 -423 3 #:row (make-eear 52 -267 3 1))
              (list -52 267))

(check-equal? (eea-lde 423 2172 3) (list -267 52))
(check-equal? (eea-lde -423 2172 3) (list 267 52))
(check-equal? (eea-lde 423 -2172 3) (list -267 -52))
(check-equal? (eea-lde -423 -2172 3) (list 267 -52))
(check-equal? (eea-lde -423 -2172 3) (list 267 -52))
(check-equal? (eea-lde -423 -2172 3 #:row (eear 52 -267 3 1))
              (list 267 -52))

(check-equal? (eea-lde 2172 423 -3) (list -52 267))
(check-equal? (eea-lde 2172 423 6) (list 104 -534))
(check-equal? (eea-lde 2172 423 7 #:row (eear 52 -267 3 1))
              false)


;; (solve-lde a b c xi yi) produces:
;; * A list of solutions (x,y) to the linear Diophantine equation, ax + by = c,
;;     where x is in the interval xi, and y is in the interval yi.
;; * A summary of the solution if there are no or many solutions (> max-result).
;; solve-lde: Int Int Int Interval Interval ->
;;   (anyof Str (list Int Int) (ne-listof (list Int Int)))
(define (solve-lde a b c xi yi)
  (cond
    [(and (zero? a) (zero? b))
     (cond [(zero? c)
            (string-append (interval->string "x" xi) ", "
                           (interval->string "y" yi))]
           [else no-soln])]

    [(zero? a)
     (define y (/ c b))
     (cond [(not (in-interval? y yi)) no-soln]
           [else (string-append (interval->string "x" xi)
                                ", y=" (number->string y))])]
          
    [(zero? b)
     (define x (/ c a))
     (cond [(not (in-interval? x xi)) no-soln]
           [else (string-append "x=" (number->string x) ", "
                                (interval->string "y" yi))])]

    [else
     (define row (eea-2nd-last-row a b))
     (define part-soln (eea-lde a b c #:row row))
          
     (cond
       [(false? part-soln) no-soln]
       [else
        (define x0 (first part-soln))
        (define y0 (second part-soln))
        (define gcd-ab (eea-gcd row))
        (define b/d (/ b gcd-ab))
        (define -a/d (/ (- a) gcd-ab))
        (define intvl
          (int-interval (solve-ineq-sys (list x0 b/d) (list y0 -a/d) xi yi)))

        (cond
          [(not (valid-interval? intvl)) no-soln]
          [else
           (define num-soln (- (interval-high intvl) (interval-low intvl) -1))

           (cond
             [(> num-soln max-result)
                    
              ;; (plus-sign n) produces "+" if n is positive or zero
              ;;   or an empty string ("") otherwise.
              ;; plus-sign: Num -> (anyof "+" "")
              (define (plus-sign n)
                (cond [(negative? n) ""]
                      [else "+"]))
                    
              (define summary
                (string-append "x=" (number->string x0) (plus-sign b/d)
                               (number->string b/d) var ", "
                               "y=" (number->string y0) (plus-sign -a/d)
                               (number->string -a/d) var ", "
                               (interval->string var intvl)))

              summary]
                   
             [else
              (define soln-lst
                (map (lambda (n) (list (+ x0 (* b/d n))
                                       (+ y0 (* -a/d n))))
                     (build-list num-soln (λ (x)
                                            (+ (interval-low intvl) x)))))

              soln-lst])])])]))

;; Tests:
(check-equal? (solve-lde 9 5 137 pos pos)
              '((3 22) (8 13) (13 4)))
(check-equal? (solve-lde -9 5 137 pos pos)
              "x=137+5n, y=274+9n, n>=-27")
(check-equal? (solve-lde 9 -5 137 pos pos)
              "x=-137-5n, y=-274-9n, n<=-31")
(check-equal? (solve-lde -9 -5 137 pos pos) no-soln)
(check-equal? (solve-lde 9 5 -137 pos pos) no-soln)
(check-equal? (solve-lde -9 5 -137 pos pos)
              "x=-137+5n, y=-274+9n, n>=31")
(check-equal? (solve-lde 9 -5 -137 pos pos)
              "x=137-5n, y=274-9n, n<=27")
(check-equal? (solve-lde -9 -5 -137 pos pos)
              '((13 4) (8 13) (3 22)))

(check-equal? (solve-lde 10 8 100 pos pos)
              (list (list 2 10) (list 6 5)))
(check-equal? (solve-lde 10 8 100 nonneg nonneg)
              (list (list 2 10) (list 6 5) (list 10 0)))

(check-equal? (solve-lde 0 0 10 real real) no-soln)
(check-equal? (solve-lde 0 0 0 real nonneg)
              "x is in the real number set, y>=0")
(check-equal? (solve-lde 0 5 10 pos pos) "x>0, y=2")
(check-equal? (solve-lde -5 0 10 neg nonpos) "x=-2, y<=0")
(check-equal? (solve-lde -5 0 10 pos nonpos) no-soln)

(check-equal? (solve-lde 10 8 99 pos pos) no-soln)
(check-equal? (solve-lde 7 21 5 neg real) no-soln)
